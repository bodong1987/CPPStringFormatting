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

#include <Format/Common/AutoArray.hpp>
#include <Format/Common/CharTraits.hpp>
#include <cassert>

namespace Formatting
{
    // Allows users to directly define the default TAutoString stack character size from the outside
#ifndef FL_DEFAULT_AUTO_STRING_STACK_LENGTH
#define FL_DEFAULT_AUTO_STRING_STACK_LENGTH 120
#endif    
    
    /// <summary>
    /// Class TAutoString.
    /// stack string
    /// Implements the <see cref="TAutoArray" />
    /// </summary>
    /// <seealso cref="TAutoArray" />
    template < typename TCharType >
    class TAutoString :
        public TAutoArray< TCharType, FL_DEFAULT_AUTO_STRING_STACK_LENGTH, 1 >
    {
    public:
        typedef TAutoArray< TCharType, FL_DEFAULT_AUTO_STRING_STACK_LENGTH, 1 > Super;
        typedef TCharTraits<TCharType>                                          CharTraits;
        typedef TCharType                                                       CharType;

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
                    CharTraits::copy(StackVal, str, Length);
                    StackVal[Count] = 0;
                }
                else
                {
                    HeapValPtr = Allocate(Length);
                    CharTraits::copy(HeapValPtr, str, Length);
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

        void AddStr(const CharType* str)
        {
            if(str == nullptr)
            {
                return;
            }

            AddStr(str, CharTraits::length(str));
        }
                        
        void AddStr(const CharType* str, const size_t length)
        {
            if(str == nullptr || length == 0)
            {
                return;
            }
            
            const size_t NewCount = Count + length;
            
            if (IsDataOnStack())
            {
                if (NewCount <= DEFAULT_LENGTH)
                {
                    CharTraits::copy(StackVal + Count, str, length);
                    Count = NewCount;

                    StackVal[Count] = 0;
                }
                else
                {
                    assert(!HeapValPtr);

                    AllocatedCount = NewCount + NewCount/2; 
                    HeapValPtr = Allocate(AllocatedCount);
                    assert(HeapValPtr);

                    if (Count > 0)
                    {
                        CharTraits::copy(HeapValPtr, StackVal, Count);
                    }

                    CharTraits::copy(HeapValPtr + Count, str, length);
                    Count = NewCount;

                    HeapValPtr[Count] = 0;
                }
            }
            else
            {
                if (NewCount <= AllocatedCount)
                {
                    CharTraits::copy(HeapValPtr + Count, str, length);                    
                    Count = NewCount;

                    HeapValPtr[Count] = 0;
                }
                else
                {
                    size_t NewAllocatedCount = NewCount + NewCount/2;

                    CharType* DataPtr = Allocate(NewAllocatedCount);

                    if (Count > 0)
                    {
                        CharTraits::copy(DataPtr, HeapValPtr, Count);
                    }

                    ReleaseHeapData();

                    assert(HeapValPtr == nullptr);

                    CharTraits::copy(DataPtr + Count, str, length);

                    Count = NewCount;

                    AllocatedCount = NewAllocatedCount;
                    HeapValPtr = DataPtr;

                    HeapValPtr[Count] = 0;
                }
            }
        }

    private:
        void AppendWithPadding(CharType* bufferPtr, const CharType* start, const size_t length, const size_t targetLength, bool paddingLeft, CharType fillChar)
        {
            const bool bNeedPadding = targetLength > length;
            const int PaddingCount = static_cast<int>(targetLength - length);

            assert(bufferPtr);
            assert(start);

            if (bNeedPadding)
            {
                if (paddingLeft)
                {
                    CharTraits::Fill(bufferPtr + Count, fillChar, PaddingCount);
                    CharTraits::copy(bufferPtr + Count + PaddingCount, start, length);
                }
                else
                {
                    CharTraits::copy(bufferPtr + Count, start, length);
                    CharTraits::Fill(bufferPtr + Count + length, fillChar, PaddingCount);
                }

                assert(PaddingCount + length == targetLength);

                Count += targetLength;

                bufferPtr[Count] = TCharTraits<CharType>::GetEndFlag();
            }
            else
            {
                CharTraits::copy(bufferPtr + Count, start, length);
                Count += length;

                bufferPtr[Count] = TCharTraits<CharType>::GetEndFlag();
            }
        }

    public:
        void AddAlignStr(const CharType* start, const size_t length, const int alignedLength, const bool paddingLeft, const CharType fillChar)
        {
            // get text length
            const size_t TargetLength = Algorithm::Max(length, static_cast<size_t>(alignedLength));            

            if (IsDataOnStack())
            {
                if (Count + TargetLength <= DEFAULT_LENGTH)
                {
                    AppendWithPadding(StackVal, start, length, TargetLength, paddingLeft, fillChar);
                }
                else
                {
                    assert(!HeapValPtr);
                    AllocatedCount = (size_t)((Count + TargetLength) * 1.5f); // NOLINT
                    HeapValPtr = Allocate(AllocatedCount);

                    assert(HeapValPtr);

                    if (Count > 0)
                    {
                        CharTraits::copy(HeapValPtr, StackVal, Count);
                    }
                    
                    AppendWithPadding(HeapValPtr, start, length, TargetLength, paddingLeft, fillChar);                    
                }
            }
            else
            {
                if (Count + TargetLength < AllocatedCount)
                {
                    AppendWithPadding(HeapValPtr, start, length, TargetLength, paddingLeft, fillChar);
                }
                else
                {
                    const size_t NewCount = (size_t)((Count + TargetLength)*1.5f); // NOLINT
                    CharType* DataPtr = Allocate(NewCount);
                    assert(DataPtr);

                    if (Count > 0)
                    {
                        CharTraits::copy(DataPtr, HeapValPtr, Count);
                    }

                    ReleaseHeapData();

                    AllocatedCount = NewCount;
                    HeapValPtr = DataPtr;

                    AppendWithPadding(HeapValPtr, start, length, TargetLength, paddingLeft, fillChar);
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

            assert(IsDataOnStack() ? (Count <= DEFAULT_LENGTH) : (Count < AllocatedCount)); // NOLINT
        }

        void Clear()
        {
            Count = 0;
            StackVal[0] = 0;
            if(HeapValPtr != nullptr)
            {
                HeapValPtr[0] = 0;
            }
        }

        bool IsEmpty() const
        {
            return Count == 0;
        }        

    protected:        
        void  AddItem(const TCharType& value)
        {
            Super::AddItem(value);
        }
    };
}
