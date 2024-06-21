#pragma once

#include <Format/Common/AutoArray.hpp>
#include <Format/Common/CharTraits.hpp>
#include <cassert>

namespace FormatLibrary
{
    template < typename TCharType >
    class TAutoString :
        public TAutoArray< TCharType, 0xFF, 2 >
    {
    public:
        typedef TAutoArray< TCharType, 0xFF, 2 > Super;
        typedef TCharTraits<TCharType>           CharTraits;
        typedef TCharType                        CharType;

        using Super::Count;
        using Super::AllocatedCount;
        using Super::HeapValPtr;
        using Super::StackVal;
        using Super::Allocate;
        using Super::IsDataOnStack;
        using Super::DEFAULT_LENGTH;
        using Super::GetDataPtr;
        using Super::ReleaseHeapData;
                
        TAutoString()
        {
            FL_STATIC_ASSERT(Super::DEFAULT_LENGTH>0, "Invalid TAutoString usage.");

            Super::StackVal[0] = 0;
        }
                
        TAutoString(const CharType* str)
        {
            if (str)
            {
                const size_t Length = CharTraits::length(str);

                Count = Length;

                if (Length <= DEFAULT_LENGTH)
                {
                    CharTraits::copy(str, str + Length, StackVal);
                    StackVal[Count] = 0;
                }
                else
                {
                    HeapValPtr = Allocate(Length);
                    CharTraits::copy(str, str + Length, HeapValPtr);
                    HeapValPtr[Count] = 0;
                }
            }
        }
                
        void  AddChar(CharType value)
        {
            AddItem(value);

            if (IsDataOnStack())
            {
                StackVal[Count] = 0;
            }
            else
            {
                HeapValPtr[Count] = 0;
            }
        }
                
        void AddStr(const CharType* start, const CharType* end = nullptr)
        {
            const size_t Length = end ? end - start : CharTraits::length(start);

            if (IsDataOnStack())
            {
                if (Count + Length <= DEFAULT_LENGTH)
                {
                    CharTraits::copy(StackVal + Count, start, Length);
                    Count += Length;

                    StackVal[Count] = 0;
                }
                else
                {
                    assert(!HeapValPtr);

                    AllocatedCount = static_cast<size_t>((Count + Length) * 1.5f);
                    HeapValPtr = Allocate(AllocatedCount);
                    assert(HeapValPtr);

                    if (Count > 0)
                    {
                        CharTraits::copy(HeapValPtr, StackVal, Count);
                    }

                    CharTraits::copy(HeapValPtr + Count, start, Length);

                    Count += Length;

                    HeapValPtr[Count] = 0;
                }
            }
            else
            {
                if (Count + Length <= AllocatedCount)
                {
                    CharTraits::copy(HeapValPtr + Count, start, Length);
                    Count += Length;

                    HeapValPtr[Count] = 0;
                }
                else
                {
                    size_t NewCount = static_cast<size_t>((Count + Length) * 1.5f);

                    CharType* DataPtr = Allocate(NewCount);

                    if (Count > 0)
                    {
                        CharTraits::copy(DataPtr, HeapValPtr, Count);
                    }

                    ReleaseHeapData();

                    CharTraits::copy(DataPtr, start, Length);

                    Count += Length;

                    AllocatedCount = NewCount;
                    HeapValPtr = DataPtr;

                    HeapValPtr[Count] = 0;
                }
            }
        }

        const TCharType* CStr() const
        {
            return GetDataPtr();
        }

        void InjectAdd(size_t count)
        {
            Count += count;

            assert(IsDataOnStack() ? (Count <= DEFAULT_LENGTH) : (Count < AllocatedCount));
        }

    protected:        
        void  AddItem(const TCharType& value)
        {
            Super::AddItem(value);
        }
    };
}
