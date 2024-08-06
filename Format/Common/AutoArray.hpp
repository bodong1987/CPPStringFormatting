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
// ReSharper disable CppDFAUnreachableCode
#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Common/Noncopyable.hpp>
#include <Format/Common/Algorithm.hpp>
#include <cassert>

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
        typedef T                                           ValueType; // NOLINT

        enum
        {            
            DEFAULT_LENGTH = DefaultLength // NOLINT
        };
                
        class ConstIterator : Noncopyable
        {
        public:
            typedef T       ValueType;
            
            explicit ConstIterator(const SelfType& referenceTarget) :
                Ref(referenceTarget),
                Index(referenceTarget.GetLength() > 0 ? 0 : -1)
            {
            }
                        
            bool IsValid() const  // NOLINT(modernize-use-nodiscard)
            {
                return Index < Ref.GetLength();
            }
                        
            void Next()
            {
                ++Index;
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
		    TakeFrom(other);
		    return *this;
        }
#endif
        bool  IsDataOnStack() const   // NOLINT(modernize-use-nodiscard)
        {
            return HeapValPtr == nullptr;
        }
                
        void AddItem(const T& value)
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
                    ExpandHeapSpace(AllocatedCount * 2);

                    assert(Count < AllocatedCount);
                    HeapValPtr[Count] = value;
                    ++Count;
                }
            }
        }

        void AddItems(const T* items, size_t length)
		{
		    assert(items != nullptr);

		    const size_t TargetCount = Count + length;
		    
		    if(IsDataOnStack())
		    {
		        if(TargetCount < DEFAULT_LENGTH)
		        {
		            Algorithm::CopyArray(items, items + length, StackVal + Count);		            
		        }
                else
                {
                    InitialMoveDataToHeap(TargetCount);

                    assert(HeapValPtr != nullptr);
                    assert(TargetCount <= AllocatedCount);
                    
                    Algorithm::CopyArray(items, items + length, HeapValPtr + Count);
                }
		    }
            else
            {
                if(TargetCount < AllocatedCount)
                {
                    Algorithm::CopyArray(items, items + length, HeapValPtr + Count);
                }
                else
                {
                    ExpandHeapSpace(TargetCount + 0xF);

                    Algorithm::CopyArray(items, items + length, HeapValPtr + Count);
                }
            }

		    Count = TargetCount;
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

        const T* GetDataPtr() const  // NOLINT(modernize-use-nodiscard)
        {
            return IsDataOnStack() ? StackVal : HeapValPtr;
        }

        T* GetUnusedPtr()
        {
            return IsDataOnStack() ? StackVal + Count : HeapValPtr + Count;
        }

        const T* GetUnusedPtr() const  // NOLINT(modernize-use-nodiscard)
        {
            return IsDataOnStack() ? StackVal + Count : HeapValPtr + Count;
        }

        size_t GetCapacity() const  // NOLINT(modernize-use-nodiscard)
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

        void Shrink()
		{
		    if(AllocatedCount <= Count || HeapValPtr == nullptr)
		    {
		        return;
		    }

		    T* DataPtr = Allocate(Count);
		    assert(DataPtr);

		    if(ExtraLength > 0)
		    {
		        memset(DataPtr + Count, 0, ExtraLength*sizeof(T));
		    }

#if FL_COMPILER_IS_GREATER_THAN_CXX11
		    Algorithm::MoveArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#else
		    Algorithm::CopyArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#endif

		    ReleaseHeapData();

		    assert(HeapValPtr == nullptr);

		    HeapValPtr = DataPtr;
		    AllocatedCount = Count;
		}

        // ReSharper disable IdentifierTypo
        // ReSharper disable CppInconsistentNaming
        typedef T*                              iterator;
        typedef const T*                        const_iterator;
        typedef std::reverse_iterator<T*>       reverse_iterator;
        typedef std::reverse_iterator<const T*> const_reverse_iterator;
        
        // support range based for
        T* begin() { return GetDataPtr(); }
        const T* begin() const { return GetDataPtr(); }

        T* end() { return GetDataPtr() + GetLength(); }
        const T* end() const { return GetDataPtr() + GetLength(); }
        
        std::reverse_iterator<T*> rbegin() { return std::reverse_iterator<T*>(end()); }        
        std::reverse_iterator<const T*> rbegin() const { return std::reverse_iterator<const T*>(end()); }

        std::reverse_iterator<T*> rend() { return std::reverse_iterator<T*>(begin()); }
        std::reverse_iterator<const T*> rend() const { return std::reverse_iterator<const T*>(begin()); }
        // ReSharper restore IdentifierTypo
        // ReSharper restore CppInconsistentNaming

    protected:
        void InitialMoveDataToHeap(const size_t initLength = DEFAULT_LENGTH * 2)
        {
            assert(HeapValPtr == nullptr);

            AllocatedCount = initLength;

            HeapValPtr = Allocate(AllocatedCount);

#if FL_COMPILER_IS_GREATER_THAN_CXX11
            Algorithm::MoveArray(StackVal, StackVal + Count, HeapValPtr);
#else
            Algorithm::CopyArray(StackVal, StackVal + Count, HeapValPtr);
#endif
        }

        void ExpandHeapSpace(const size_t newCount)
        {
            const size_t NewCount = newCount <= AllocatedCount ? AllocatedCount * 2 : newCount;
            assert(NewCount > AllocatedCount);

            T* DataPtr = Allocate(NewCount);

            assert(DataPtr);

#if FL_COMPILER_IS_GREATER_THAN_CXX11
            Algorithm::MoveArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#else
            Algorithm::CopyArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#endif

            ReleaseHeapData();

            assert(HeapValPtr == nullptr);

            HeapValPtr = DataPtr;
            AllocatedCount = NewCount;
        }

        void ReleaseHeapData()
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

        // ReSharper disable once CppRedundantAccessSpecifier
    protected:
        size_t        Count;
        size_t        AllocatedCount;
        T             StackVal[DEFAULT_LENGTH + ExtraLength];
        T*            HeapValPtr;
    };
}
