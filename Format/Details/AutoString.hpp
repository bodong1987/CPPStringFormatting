#pragma once

#include <Format/Details/AutoArray.hpp>
#include <Format/Details/Traits.hpp>

namespace FormatLibrary
{
    namespace Utility
    {
        /// <summary>
        /// Class TAutoString.
        /// String Wrapper
        /// Implements the <see cref="TAutoArray{TCharType, 0xFF, 2}" />
        /// </summary>
        /// <seealso cref="TAutoArray{TCharType, 0xFF, 2}" />
        template < typename TCharType >
        class TAutoString :
            public TAutoArray< TCharType, 0xFF, 2 >
        {
        public:
            typedef TAutoArray< TCharType, 0xFF, 2 > Super;
            typedef Details::TCharTraits<TCharType>  CharTraits;
            typedef TCharType                        CharType;

#if !FL_COMPILER_MSVC
            using Super::Count;
            using Super::AllocatedCount;
            using Super::HeapValPtr;
            using Super::StackVal;
            using Super::Allocate;
            using Super::IsDataOnStack;
            using Super::DEFAULT_LENGTH;
            using Super::GetDataPtr;
            using Super::ReleaseHeapData;
#endif

            /// <summary>
            /// Initializes a new instance of the <see cref="TAutoString"/> class.
            /// </summary>
            TAutoString()
            {
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="TAutoString"/> class.
            /// </summary>
            /// <param name="str">The string.</param>
            TAutoString(const CharType* str)
            {
                if (str)
                {
                    const SIZE_T Length = CharTraits::length(str);

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

            /// <summary>
            /// Adds the character.
            /// </summary>
            /// <param name="value">The value.</param>
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

            /// <summary>
            /// Adds the string.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            void AddStr(const CharType* start, const CharType* end = NULL)
            {
                const SIZE_T Length = end ? end - start : CharTraits::length(start);

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

                        AllocatedCount = static_cast<SIZE_T>((Count + Length) * 1.5f);
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
                        SIZE_T NewCount = static_cast<SIZE_T>((Count + Length) * 1.5f);

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

            /// <summary>
            /// cs the string.
            /// </summary>
            /// <returns>const TCharType *.</returns>
            const TCharType* CStr() const
            {
                return GetDataPtr();
            }

            /// <summary>
            /// Injects the add. it is a internal function
            /// </summary>
            /// <param name="count">The count.</param>
            void InjectAdd(SIZE_T count)
            {
                Count += count;

                assert(IsDataOnStack() ? (Count <= DEFAULT_LENGTH) : (Count < AllocatedCount));
            }

        protected:
            /// <summary>
            /// Adds the item.
            /// </summary>
            /// <param name="value">The value.</param>
            void  AddItem(const TCharType& value)
            {
                Super::AddItem(value);
            }
        };
    }
}
