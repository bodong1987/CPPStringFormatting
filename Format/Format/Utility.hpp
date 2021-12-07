// FormatLibrary
// @author bodong
// Utility Classes
#pragma once

// depends on algorithm
#include <algorithm>

namespace FormatLibrary
{
    namespace Algorithm
    {
        namespace Detail
        {
            /// <summary>
            /// Moves the array implementation.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="dest">The dest.</param>
            /// <param name="">The .</param>
            /// <returns>OutputType.</returns>
            template <typename InputType, typename OutputType >
            inline OutputType MoveArrayImpl(InputType start, InputType end, OutputType dest, Mpl::FalseType)
            {
                while (start != end)
                {
                    *dest = FL_MOVE_SEMANTIC(*start);
                    ++dest;
                    ++start;
                }

                return dest;
            }

            /// <summary>
            /// Moves the array implementation.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="dest">The dest.</param>
            /// <param name="">The .</param>
            /// <returns>T *.</returns>
            template <typename T >
            inline T* MoveArrayImpl(T* start, T* end, T* dest, Mpl::TrueType)
            {
                const size_t Size = (end - start)*sizeof(T);

                memcpy(dest, start, Size);

                return dest + (end - start);
            }

            /// <summary>
            /// Copies the array implementation.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="dest">The dest.</param>
            /// <param name="">The .</param>
            /// <returns>OutputType.</returns>
            template <typename InputType, typename OutputType >
            inline OutputType CopyArrayImpl(InputType start, InputType end, OutputType dest, Mpl::FalseType)
            {
                while (start != end)
                {
                    *dest = *start;
                    ++dest;
                    ++start;
                }

                return dest;
            }

            /// <summary>
            /// Copies the array implementation.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="dest">The dest.</param>
            /// <param name="">The .</param>
            /// <returns>T *.</returns>
            template <typename T >
            inline T* CopyArrayImpl(T* start, T* end, T* dest, Mpl::TrueType)
            {
                const size_t Size = (end - start)*sizeof(T);

                memcpy(dest, start, Size);

                return dest + (end - start);
            }
        }

        /// <summary>
        /// Moves the array.
        /// </summary>
        /// <param name="start">The start.</param>
        /// <param name="end">The end.</param>
        /// <param name="dest">The dest.</param>
        /// <returns>OutputType.</returns>
        template<typename InputType, typename OutputType>
        inline OutputType MoveArray(InputType start, InputType end, OutputType dest)
        {
            typedef typename Mpl::TIfElse<
                Mpl::IsSimple<InputType>::Value &&
                Mpl::IsSame<InputType, OutputType>::Value,
                Mpl::TrueType,
                Mpl::FalseType >::Type  Type;

            return Detail::MoveArrayImpl(start, end, dest, Type());
        }

        /// <summary>
        /// Copies the array.
        /// </summary>
        /// <param name="start">The start.</param>
        /// <param name="end">The end.</param>
        /// <param name="dest">The dest.</param>
        /// <returns>OutputType.</returns>
        template<typename InputType, typename OutputType>
        inline OutputType CopyArray(InputType start, InputType end, OutputType dest)
        {
            typedef typename Mpl::TIfElse<
                Mpl::IsSimple<InputType>::Value &&
                Mpl::IsSame<InputType, OutputType>::Value,
                Mpl::TrueType,
                Mpl::FalseType >::Type  Type;

            return Detail::CopyArrayImpl(start, end, dest, Type());
        }

        /// <summary>
        /// Clamps the specified x.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="minValue">The minimum value.</param>
        /// <param name="maxValue">The maximum value.</param>
        /// <returns>T.</returns>
        template< class T > inline T Clamp(const T x, const T minValue, const T maxValue)
        {
            return x < minValue ? minValue : x < maxValue ? x : maxValue;
        }
    }

    namespace Utility
    {
        /// <summary>
        /// Class Noncopyable.
        /// </summary>
        class Noncopyable
        {
        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="Noncopyable"/> class.
            /// </summary>
            Noncopyable() {}
            /// <summary>
            /// Finalizes an instance of the <see cref="Noncopyable"/> class.
            /// </summary>
            ~Noncopyable() {}
        protected:
#if FL_COMPILER_WITH_CXX11
            Noncopyable(const Noncopyable&) = delete;
            Noncopyable& operator = (const Noncopyable&) = delete;
#else
            /// <summary>
            /// Initializes a new instance of the <see cref="Noncopyable"/> class.
            /// </summary>
            /// <param name="">The .</param>
            Noncopyable(const Noncopyable&);
            /// <summary>
            /// Operator=s the specified .
            /// </summary>
            /// <param name="">The .</param>
            /// <returns>FormatLibrary.Utility.Noncopyable &.</returns>
            Noncopyable& operator = (const Noncopyable&);
#endif
        };

        /// <summary>
        /// Class TScopedLocker.
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        template < typename TReferenceType >
        class TScopedLocker :
            Noncopyable
        {
        public:
            typedef TReferenceType        ReferenceType;
            
            /// <summary>
            /// Initializes a new instance of the <see cref="TScopedLocker"/> class.
            /// </summary>
            /// <param name="referenceTarget">The reference target.</param>
            TScopedLocker(TReferenceType& referenceTarget) :
                Reference(&referenceTarget)
            {
                assert(Reference);
                Reference->Lock();
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="TScopedLocker"/> class.
            /// </summary>
            /// <param name="referencePointer">The reference pointer.</param>
            TScopedLocker(TReferenceType* referencePointer) :
                Reference(referencePointer)
            {
                assert(Reference);
                Reference->Lock();
            }

            /// <summary>
            /// Finalizes an instance of the <see cref="TScopedLocker"/> class.
            /// </summary>
            ~TScopedLocker()
            {
                Reference->UnLock();
            }

        protected:
            /// <summary>
            /// The reference
            /// </summary>
            TReferenceType* Reference;
        };

        /// <summary>
        /// Class CriticalSectionNone.
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        class CriticalSectionNone :
            Noncopyable
        {
        public:
            /// <summary>
            /// Locks this instance.
            /// </summary>
            void Lock() {}
            /// <summary>
            /// Uns the lock.
            /// </summary>
            void UnLock() {}
        };

        /// <summary>
        /// Class TAutoArray.
        /// </summary>
        template <
            typename T,
            INT DefaultLength = 0xFF,
            INT ExtraLength = 0
        >
        class TAutoArray
        {
        public:
            typedef TAutoArray<T, DefaultLength, ExtraLength>   SelfType;

            enum
            {
                /// <summary>
                /// The default length
                /// </summary>
                DEFAULT_LENGTH = DefaultLength
            };

            /// <summary>
            /// Class ConstIterator.
            /// Implements the <see cref="Noncopyable" />
            /// </summary>
            /// <seealso cref="Noncopyable" />
            class ConstIterator : Noncopyable
            {
            public:
                /// <summary>
                /// Initializes a new instance of the <see cref="TAutoArray"/> class.
                /// </summary>
                /// <param name="referenceTarget">The reference target.</param>
                ConstIterator(const SelfType& referenceTarget) :
                    Ref(referenceTarget),
                    index( referenceTarget.GetLength()>0?0:-1 )
                {
                }

                /// <summary>
                /// Returns true if ... is valid.
                /// </summary>
                /// <returns>bool.</returns>
                bool IsValid() const
                {
                    return index < Ref.GetLength();
                }

                /// <summary>
                /// Nexts this instance.
                /// </summary>
                /// <returns>bool.</returns>
                bool Next()
                {
                    ++index;

                    return IsValid();
                }

                /// <summary>
                /// Operator*s this instance.
                /// </summary>
                /// <returns>const T &.</returns>
                const T& operator *() const
                {
                    const T* Ptr = Ref.GetDataPtr();

                    return Ptr[index];
                }
            private:
                /// <summary>
                /// Operator=s the specified .
                /// </summary>
                /// <param name="">The .</param>
                /// <returns>FormatLibrary.Utility.TAutoArray&lt;T, DefaultLength, ExtraLength&gt;.ConstIterator &.</returns>
                ConstIterator& operator = (const ConstIterator&);
                /// <summary>
                /// Initializes a new instance of the <see cref="TAutoArray"/> class.
                /// </summary>
                /// <param name="">The .</param>
                ConstIterator(ConstIterator&);
            protected:
                /// <summary>
                /// The reference
                /// </summary>
                const SelfType& Ref;
                /// <summary>
                /// The index
                /// </summary>
                SIZE_T            index;
            };

            /// <summary>
            /// Initializes a new instance of the <see cref="TAutoArray"/> class.
            /// </summary>
            TAutoArray() :
                Count(0),
                AllocatedCount(0),
                HeapValPtr(NULL)
            {
            }

            /// <summary>
            /// Finalizes an instance of the <see cref="TAutoArray"/> class.
            /// </summary>
            ~TAutoArray()
            {
                ReleaseHeapData();

                Count = 0;
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="TAutoArray"/> class.
            /// </summary>
            /// <param name="other">The other.</param>
            TAutoArray(const SelfType& other) :
                Count(other.Count),
                AllocatedCount(other.AllocatedCount),
                HeapValPtr(NULL)
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

            /// <summary>
            /// Operator=s the specified other.
            /// </summary>
            /// <param name="other">The other.</param>
            /// <returns>SelfType &.</returns>
            SelfType& operator = (const SelfType& other)
            {
                if (this == &other)
                {
                    return *this;
                }

                ReleaseHeapData();

                Count = other.Count;
                AllocatedCount = other.AllocatedCount;
                HeapValPtr = NULL;

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

            /// <summary>
            /// Takes from.
            /// </summary>
            /// <param name="other">The other.</param>
            /// <returns>SelfType &.</returns>
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
                other.HeapValPtr = NULL;
            }

            /// <summary>
            /// Takes to.
            /// </summary>
            /// <param name="other">The other.</param>
            void TakeTo(SelfType& other)
            {
                other.TakeFrom(*this);
            }

#if FL_COMPILER_HAS_RIGHT_VALUE_REFERENCE
            /// <summary>
            /// Initializes a new instance of the <see cref="TAutoArray"/> class.
            /// </summary>
            /// <param name="other">The other.</param>
            TAutoArray(SelfType&& other) :
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
                other.HeapValPtr = NULL;
            }

            /// <summary>
            /// Operator=s the specified other.
            /// </summary>
            /// <param name="other">The other.</param>
            /// <returns>SelfType &.</returns>
            SelfType& operator = (SelfType&& other)
            {
                return TakeFrom(other);
            }
#endif

            /// <summary>
            /// Determines whether [is data on stack].
            /// </summary>
            /// <returns>bool.</returns>
            bool  IsDataOnStack() const
            {
                return HeapValPtr == NULL;
            }

            /// <summary>
            /// Adds the item.
            /// </summary>
            /// <param name="value">The value.</param>
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

            /// <summary>
            /// Gets the length.
            /// </summary>
            /// <returns>SIZE_T.</returns>
            SIZE_T GetLength() const
            {
                return Count;
            }

            /// <summary>
            /// Gets the allocated count.
            /// </summary>
            /// <returns>SIZE_T.</returns>
            SIZE_T GetAllocatedCount() const
            {
                return AllocatedCount;
            }

            /// <summary>
            /// Gets the data PTR.
            /// </summary>
            /// <returns>T *.</returns>
            T* GetDataPtr()
            {
                return IsDataOnStack() ? StackVal : HeapValPtr;
            }

            /// <summary>
            /// Gets the data PTR.
            /// </summary>
            /// <returns>const T *.</returns>
            const T* GetDataPtr() const
            {
                return IsDataOnStack() ? StackVal : HeapValPtr;
            }

            /// <summary>
            /// Gets the unused PTR.
            /// </summary>
            /// <returns>T *.</returns>
            T* GetUnusedPtr()
            {
                return IsDataOnStack() ? StackVal + Count : HeapValPtr + Count;
            }

            /// <summary>
            /// Gets the unused PTR.
            /// </summary>
            /// <returns>const T *.</returns>
            const T* GetUnusedPtr() const
            {
                return IsDataOnStack() ? StackVal + Count : HeapValPtr + Count;
            }

            /// <summary>
            /// Gets the capacity.
            /// </summary>
            /// <returns>SIZE_T.</returns>
            SIZE_T GetCapacity() const
            {                
                return IsDataOnStack() ?
                    DEFAULT_LENGTH - Count :
                    AllocatedCount - Count;
            }
            
            /// <summary>
            /// Operator[]s the specified index.
            /// </summary>
            /// <param name="index">The index.</param>
            /// <returns>T &.</returns>
            T& operator [](SIZE_T index)
            {
                assert( index < GetLength() );
                
                return GetDataPtr()[index];
            }
            
            /// <summary>
            /// Operator[]s the specified index.
            /// </summary>
            /// <param name="index">The index.</param>
            /// <returns>const T &.</returns>
            const T& operator [](SIZE_T index) const
            {
                assert( index < GetLength() );
                
                return GetDataPtr()[index];
            }

        protected:
            /// <summary>
            /// Initials the move data to heap.
            /// </summary>
            void  InitialMoveDataToHeap()
            {
                assert(HeapValPtr == NULL);

                AllocatedCount = DEFAULT_LENGTH * 2;

                HeapValPtr = Allocate(AllocatedCount);

#if FL_COMPILER_HAS_RIGHT_VALUE_REFERENCE
                Algorithm::MoveArray(StackVal, StackVal + Count, HeapValPtr);
#else
                Algorithm::CopyArray(StackVal, StackVal + Count, HeapValPtr);
#endif
            }

            /// <summary>
            /// Expands the heap space.
            /// </summary>
            void  ExpandHeapSpace()
            {
                SIZE_T NewCount = AllocatedCount * 2;
                assert(NewCount > AllocatedCount);

                T* DataPtr = Allocate(NewCount);

                assert(DataPtr);

#if FL_COMPILER_HAS_RIGHT_VALUE_REFERENCE
                Algorithm::MoveArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#else
                Algorithm::CopyArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#endif
                ReleaseHeapData();

                HeapValPtr = DataPtr;
                AllocatedCount = NewCount;
            }

            /// <summary>
            /// Releases the heap data.
            /// </summary>
            void  ReleaseHeapData()
            {
                if (HeapValPtr)
                {
                    delete[] HeapValPtr;
                    HeapValPtr = NULL;
                }

                AllocatedCount = 0;
            }

            /// <summary>
            /// Allocates the specified in allocated count.
            /// </summary>
            /// <param name="InAllocatedCount">The in allocated count.</param>
            /// <returns>T *.</returns>
            static T* Allocate(const SIZE_T InAllocatedCount)
            {
                // +ExtraLength this is a hack method for saving string on it.
                return new T[InAllocatedCount + ExtraLength];
            }

        protected:
            /// <summary>
            /// The count
            /// </summary>
            SIZE_T        Count;
            /// <summary>
            /// The allocated count
            /// </summary>
            SIZE_T        AllocatedCount;

            /// <summary>
            /// +ExtraLength this is a hack method for saving string on it.
            /// </summary>
            T             StackVal[DEFAULT_LENGTH + ExtraLength];

            /// <summary>
            /// The heap value PTR
            /// </summary>
            T* HeapValPtr;
        };

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
            typedef Mpl::TCharTraits<TCharType>      CharTraits;
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
                        CharTraits::copy(StackVal+Count, start, Length);
                        Count += Length;

                        StackVal[Count] = 0;
                    }
                    else
                    {
                        assert(!HeapValPtr);

                        AllocatedCount = static_cast<SIZE_T>((Count + Length)*1.5f);
                        HeapValPtr = Allocate(AllocatedCount);
                        assert(HeapValPtr);

                        if (Count > 0)
                        {
                            CharTraits::copy(HeapValPtr, StackVal, Count);
                        }

                        CharTraits::copy(HeapValPtr+Count, start, Length);

                        Count += Length;

                        HeapValPtr[Count] = 0;
                    }
                }
                else
                {
                    if (Count + Length <= AllocatedCount)
                    {
                        CharTraits::copy(HeapValPtr+Count, start, Length);
                        Count += Length;

                        HeapValPtr[Count] = 0;
                    }
                    else
                    {
                        SIZE_T NewCount = static_cast<SIZE_T>((Count + Length)*1.5f);

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

        /// <summary>
        /// Lengthes the of.
        /// </summary>
        /// <param name="pstr">The PSTR.</param>
        /// <returns>size_t.</returns>
        inline size_t LengthOf(const wchar_t* pstr)
        {
            return wcslen(pstr);
        }

        /// <summary>
        /// Lengthes the of.
        /// </summary>
        /// <param name="pstr">The PSTR.</param>
        /// <returns>size_t.</returns>
        inline size_t LengthOf(const char* pstr)
        {
            return strlen(pstr);
        }

        /// <summary>
        /// Lengthes the of.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns>size_t.</returns>
        inline size_t LengthOf(const std::basic_string<char>& str)
        {
            return str.length();
        }

        /// <summary>
        /// Lengthes the of.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns>size_t.</returns>
        inline size_t LengthOf(const std::basic_string<wchar_t>& str)
        {
            return str.length();
        }

        /// <summary>
        /// PTRs the of.
        /// </summary>
        /// <param name="pstr">The PSTR.</param>
        /// <returns>const wchar_t *.</returns>
        inline const wchar_t* PtrOf(const wchar_t* pstr)
        {
            return pstr;
        }

        /// <summary>
        /// PTRs the of.
        /// </summary>
        /// <param name="pstr">The PSTR.</param>
        /// <returns>const char *.</returns>
        inline const char* PtrOf(const char* pstr)
        {
            return pstr;
        }

        /// <summary>
        /// PTRs the of.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns>const char *.</returns>
        inline const char* PtrOf(const std::basic_string<char>& str)
        {
            return str.c_str();
        }

        /// <summary>
        /// PTRs the of.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns>const wchar_t *.</returns>
        inline const wchar_t* PtrOf(const std::basic_string<wchar_t>& str)
        {
            return str.c_str();
        }
    }
}
