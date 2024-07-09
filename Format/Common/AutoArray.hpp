/*
    MIT License

    Copyright (c) 2024 CPPStringFormatting

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    Project URL: https://github.com/bodong1987/CPPStringFormatting
*/
#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Common/Noncopyable.hpp>
#include <Format/Common/Algorithm.hpp>

namespace Formatting
{
    /// <summary>
    /// Class TAutoArray.
    /// a dynamic array can be place on stack memory if the count is less than DefaultLength
    /// </summary>
    template <
        typename T,
        int32_t DefaultLength = 0xFF,
        int32_t ExtraLength = 0
    >
    class TAutoArray
    {
    public:
        typedef TAutoArray<T, DefaultLength, ExtraLength>   SelfType; // NOLINT

        enum
        {            
            DEFAULT_LENGTH = DefaultLength // NOLINT
        };
                
        class ConstIterator : Noncopyable
        {
        public:            
            ConstIterator(const SelfType& referenceTarget) :
                Ref(referenceTarget),
                Index(referenceTarget.GetLength() > 0 ? 0 : -1)
            {
            }
                        
            bool IsValid() const
            {
                return Index < Ref.GetLength();
            }
                        
            bool Next()
            {
                ++Index;

                return IsValid();
            }
                        
            const T& operator *() const
            {
                const T* Ptr = Ref.GetDataPtr();

                return Ptr[Index];
            }
        protected:            
            const SelfType&   Ref;         // NOLINT   
            size_t            Index;
        };

		TAutoArray() :
			Count(0),
			AllocatedCount(0),
            HeapValPtr(nullptr)

        {
        }
                
        ~TAutoArray()
        {
            ReleaseHeapData();

            Count = 0;
        }

        TAutoArray(const SelfType& other) :
            Count(other.Count),
            AllocatedCount(other.AllocatedCount),
            HeapValPtr(nullptr)
        {
            if (Count > 0)
            {
                if (other.IsDataOnStack())
                {
                    Algorithm::CopyArray(other.StackVal, other.StackVal + Count, StackVal);
                }
                else
                {
                    HeapValPtr = Allocate(AllocatedCount);
                    Algorithm::CopyArray(other.HeapValPtr, other.HeapValPtr + Count, HeapValPtr);
                }
            }
        }

        SelfType& operator = (const SelfType& other) // NOLINT
        {
            if (this == &other)
            {
                return *this;
            }

            ReleaseHeapData();

            Count = other.Count;
            AllocatedCount = other.AllocatedCount;
            HeapValPtr = nullptr;

            if (Count > 0)
            {
                if (other.IsDataOnStack())
                {
                    Algorithm::CopyArray(other.StackVal, other.StackVal + Count, StackVal);
                }
                else
                {
                    HeapValPtr = Allocate(AllocatedCount);
                    Algorithm::CopyArray(other.HeapValPtr, other.HeapValPtr + Count, HeapValPtr);
                }
            }

            return *this;
        }

        SelfType& TakeFrom(SelfType& other)
        {
            if (this == &other)
            {
                return *this;
            }

            Count = other.Count;
            AllocatedCount = other.AllocatedCount;
            HeapValPtr = other.HeapValPtr;

            if (Count > 0 && other.IsDataOnStack())
            {
                Algorithm::MoveArray(other.StackVal, other.StackVal + Count, StackVal);
            }

            other.Count = 0;
            other.AllocatedCount = 0;
            other.HeapValPtr = nullptr;

            return *this;
        }

#if FL_COMPILER_IS_GREATER_THAN_CXX11
        SelfType& TakeFrom(SelfType&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            Count = other.Count;
            AllocatedCount = other.AllocatedCount;
            HeapValPtr = other.HeapValPtr;

            if (Count > 0 && other.IsDataOnStack())
            {
                Algorithm::MoveArray(other.StackVal, other.StackVal + Count, StackVal);
            }

            other.Count = 0;
            other.AllocatedCount = 0;
            other.HeapValPtr = nullptr;

            return *this;
        }
#endif

        void TakeTo(SelfType& other)
        {
            other.TakeFrom(*this);
        }

#if FL_COMPILER_IS_GREATER_THAN_CXX11
        TAutoArray(SelfType&& other) noexcept :
            Count(other.Count),
            AllocatedCount(other.AllocatedCount),
            HeapValPtr(other.HeapValPtr)
        {
            if (Count > 0 && other.IsDataOnStack())
            {
                Algorithm::MoveArray(other.StackVal, other.StackVal + Count, StackVal);
            }

            other.Count = 0;
            other.AllocatedCount = 0;
            other.HeapValPtr = nullptr;
        }

        SelfType& operator = (SelfType&& other) noexcept
        {
            return TakeFrom(other);
        }
#endif

        bool  IsDataOnStack() const
        {
            return HeapValPtr == nullptr;
        }
                
        void  AddItem(const T& value)
        {
            if (IsDataOnStack())
            {
                if (Count < DEFAULT_LENGTH)
                {
                    StackVal[Count] = value;
                    ++Count;
                }
                else if (Count == DEFAULT_LENGTH)
                {
                    InitialMoveDataToHeap();

                    assert(Count < AllocatedCount);

                    HeapValPtr[Count] = value;
                    ++Count;
                }
                else
                {
                    assert(false && "internal error");
                }
            }
            else
            {
                if (Count < AllocatedCount)
                {
                    HeapValPtr[Count] = value;
                    ++Count;
                }
                else
                {
                    ExpandHeapSpace();

                    assert(Count < AllocatedCount);
                    HeapValPtr[Count] = value;
                    ++Count;
                }
            }
        }

        size_t GetLength() const
        {
            return Count;
        }

        size_t GetAllocatedCount() const
        {
            return AllocatedCount;
        }

        T* GetDataPtr()
        {
            return IsDataOnStack() ? StackVal : HeapValPtr;
        }

        const T* GetDataPtr() const
        {
            return IsDataOnStack() ? StackVal : HeapValPtr;
        }

        T* GetUnusedPtr()
        {
            return IsDataOnStack() ? StackVal + Count : HeapValPtr + Count;
        }

        const T* GetUnusedPtr() const
        {
            return IsDataOnStack() ? StackVal + Count : HeapValPtr + Count;
        }

        size_t GetCapacity() const
        {
            return IsDataOnStack() ?
                DEFAULT_LENGTH - Count :
                AllocatedCount - Count;
        }

        T& operator [](size_t index)
        {
            assert(index < GetLength());

            return GetDataPtr()[index];
        }
        
        const T& operator [](size_t index) const
        {
            assert(index < GetLength());

            return GetDataPtr()[index];
        }

    protected:
        void  InitialMoveDataToHeap()
        {
            assert(HeapValPtr == nullptr);

            AllocatedCount = DEFAULT_LENGTH * 2;

            HeapValPtr = Allocate(AllocatedCount);

#if FL_COMPILER_HAS_RIGHT_VALUE_REFERENCE
            Algorithm::MoveArray(StackVal, StackVal + Count, HeapValPtr);
#else
            Algorithm::CopyArray(StackVal, StackVal + Count, HeapValPtr);
#endif
        }

        void  ExpandHeapSpace()
        {
            const size_t NewCount = AllocatedCount * 2;
            assert(NewCount > AllocatedCount);

            T* DataPtr = Allocate(NewCount);

            assert(DataPtr);

#if FL_COMPILER_HAS_RIGHT_VALUE_REFERENCE
            Algorithm::MoveArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#else
            Algorithm::CopyArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#endif

            ReleaseHeapData();

            assert(HeapValPtr == nullptr);

            HeapValPtr = DataPtr;
            AllocatedCount = NewCount;
        }

        void  ReleaseHeapData()
        {
            if (HeapValPtr)
            {
                delete[] HeapValPtr;
                HeapValPtr = nullptr;
            }

            AllocatedCount = 0;
        }

        static T* Allocate(const size_t allocatedCount)
        {
            // +ExtraLength this is a hack method for saving string on it.
            return new T[allocatedCount + ExtraLength];
        }

    protected:
        size_t        Count;
        size_t        AllocatedCount;
        T             StackVal[DEFAULT_LENGTH + ExtraLength];
        T*            HeapValPtr;
    };
}
