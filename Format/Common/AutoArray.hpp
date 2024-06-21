#pragma once

#include <cstdint>
#include <cstddef>
#include <algorithm>

#include <Format/Common/Noncopyable.hpp>

namespace FormatLibrary
{
    template <
        typename T,
        int32_t DefaultLength = 0xFF,
        int32_t ExtraLength = 0
    >
    class TAutoArray
    {
    public:
        typedef TAutoArray<T, DefaultLength, ExtraLength>   SelfType;

        enum
        {            
            DEFAULT_LENGTH = DefaultLength
        };
                
        class ConstIterator : Noncopyable
        {
        public:            
            ConstIterator(const SelfType& referenceTarget) :
                Ref(referenceTarget),
                index(referenceTarget.GetLength() > 0 ? 0 : -1)
            {
            }
                        
            bool IsValid() const
            {
                return index < Ref.GetLength();
            }
                        
            bool Next()
            {
                ++index;

                return IsValid();
            }
                        
            const T& operator *() const
            {
                const T* Ptr = Ref.GetDataPtr();

                return Ptr[index];
            }
        private:
            ConstIterator& operator = (const ConstIterator&) = delete;         
            ConstIterator(ConstIterator&) = delete;
        protected:            
            const SelfType&   Ref;            
            size_t            index;
        };

        TAutoArray()
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
                    std::copy(other.StackVal, other.StackVal + Count, StackVal);
                }
                else
                {
                    HeapValPtr = Allocate(AllocatedCount);
                    std::copy(other.HeapValPtr, other.HeapValPtr + Count, HeapValPtr);
                }
            }
        }

        SelfType& operator = (const SelfType& other)
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
                    std::copy(other.StackVal, other.StackVal + Count, StackVal);
                }
                else
                {
                    HeapValPtr = Allocate(AllocatedCount);
                    std::copy(other.HeapValPtr, other.HeapValPtr + Count, HeapValPtr);
                }
            }

            return *this;
        }

        SelfType& TakeFrom(SelfType&& other)
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
                std::move(other.StackVal, other.StackVal + Count, StackVal);
            }

            other.Count = 0;
            other.AllocatedCount = 0;
            other.HeapValPtr = nullptr;
        }

        void TakeTo(SelfType& other)
        {
            other.TakeFrom(*this);
        }

        TAutoArray(SelfType&& other) :
            Count(other.Count),
            AllocatedCount(other.AllocatedCount),
            HeapValPtr(other.HeapValPtr)
        {
            if (Count > 0 && other.IsDataOnStack())
            {
                std::move(other.StackVal, other.StackVal + Count, StackVal);
            }

            other.Count = 0;
            other.AllocatedCount = 0;
            other.HeapValPtr = nullptr;
        }

        SelfType& operator = (SelfType&& other)
        {
            return TakeFrom(other);
        }

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

            std::move(StackVal, StackVal + Count, HeapValPtr);
        }

        void  ExpandHeapSpace()
        {
            size_t NewCount = AllocatedCount * 2;
            assert(NewCount > AllocatedCount);

            T* DataPtr = Allocate(NewCount);

            assert(DataPtr);

            std::move(HeapValPtr, HeapValPtr + Count, DataPtr);

            ReleaseHeapData();

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

        static T* Allocate(const size_t InAllocatedCount)
        {
            // +ExtraLength this is a hack method for saving string on it.
            return new T[InAllocatedCount + ExtraLength];
        }

    protected:
        size_t        Count = 0;
        size_t        AllocatedCount = 0;
        T             StackVal[DEFAULT_LENGTH + ExtraLength];
        T*            HeapValPtr = nullptr;
    };
}
