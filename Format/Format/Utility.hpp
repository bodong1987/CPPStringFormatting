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

            template <typename T >
            inline T* MoveArrayImpl(T* start, T* end, T* dest, Mpl::TrueType)
            {
                const size_t Size = (end - start)*sizeof(T);

                memcpy(dest, start, Size);

                return dest + (end - start);
            }

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

            template <typename T >
            inline T* CopyArrayImpl(T* start, T* end, T* dest, Mpl::TrueType)
            {
                const size_t Size = (end - start)*sizeof(T);

                memcpy(dest, start, Size);

                return dest + (end - start);
            }
        }

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

        template< class T > inline T Clamp(const T x, const T minValue, const T maxValue)
        {
            return x < minValue ? minValue : x < maxValue ? x : maxValue;
        }
    }

    namespace Utility
    {
        class Noncopyable
        {
        public:
            Noncopyable(){}
            ~Noncopyable(){}
        protected:
            Noncopyable(const Noncopyable&);
            Noncopyable& operator = (const Noncopyable&);
        };

        template < typename TReferenceType >
        class TScopedLocker :
            Noncopyable
        {
        public:
            typedef TReferenceType        ReferenceType;
            
            TScopedLocker(TReferenceType& referenceTarget) :
                Reference(&referenceTarget)
            {
                assert(Reference);
                Reference->Lock();
            }

            TScopedLocker(TReferenceType* referencePointer) :
                Reference(referencePointer)
            {
                assert(Reference);
                Reference->Lock();
            }

            ~TScopedLocker()
            {
                Reference->UnLock();
            }

        protected:
            TReferenceType*   Reference;
        };

        class CriticalSectionNone :
            Noncopyable
        {
        public:
            void Lock(){}
            void UnLock(){}
        };

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
                DEFAULT_LENGTH = DefaultLength
            };

            class ConstIterator : Noncopyable
            {
            public:
                ConstIterator(const SelfType& referenceTarget) :
                    Ref(referenceTarget),
                    index( referenceTarget.GetLength()>0?0:-1 )
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
                ConstIterator& operator = (const ConstIterator&);
                ConstIterator(ConstIterator&);
            protected:
                const SelfType&   Ref;
                SIZE_T            index;
            };

            TAutoArray() :
                Count(0),
                AllocatedCount(0),
                HeapValPtr(NULL)
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

            void TakeTo(SelfType& other)
            {
                other.TakeFrom(*this);
            }

#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
            TAutoArray( SelfType && other ) :
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

            SelfType& operator = (SelfType&& other )
            {
                return TakeFrom(other);
            }
#endif

            bool  IsDataOnStack() const
            {
                return HeapValPtr == NULL;
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

            SIZE_T GetLength() const
            {
                return Count;
            }

            SIZE_T GetAllocatedCount() const
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

            SIZE_T GetCapacity() const
            {                
                return IsDataOnStack() ?
                    DEFAULT_LENGTH - Count :
                    AllocatedCount - Count;
            }
            
            T& operator []( SIZE_T index )
            {
                assert( index < GetLength() );
                
                return GetDataPtr()[index];
            }
            
            const T& operator []( SIZE_T index ) const
            {
                assert( index < GetLength() );
                
                return GetDataPtr()[index];
            }

        protected:
            void  InitialMoveDataToHeap()
            {
                assert(HeapValPtr == NULL);

                AllocatedCount = DEFAULT_LENGTH * 2;

                HeapValPtr = Allocate(AllocatedCount);

#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
                Algorithm::MoveArray(StackVal, StackVal + Count, HeapValPtr);
#else
                Algorithm::CopyArray(StackVal, StackVal + Count, HeapValPtr);
#endif
            }

            void  ExpandHeapSpace()
            {
                SIZE_T NewCount = AllocatedCount * 2;
                assert(NewCount > AllocatedCount);

                T* DataPtr = Allocate(NewCount);

                assert(DataPtr);

#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
                Algorithm::MoveArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#else
                Algorithm::CopyArray(HeapValPtr, HeapValPtr + Count, DataPtr);
#endif
                ReleaseHeapData();

                HeapValPtr = DataPtr;
                AllocatedCount = NewCount;
            }

            void  ReleaseHeapData()
            {
                if (HeapValPtr)
                {
                    delete[] HeapValPtr;
                    HeapValPtr = NULL;
                }

                AllocatedCount = 0;
            }

            static T*  Allocate(const SIZE_T InAllocatedCount)
            {
                // +ExtraLength this is a hack method for saving string on it.
                return new T[InAllocatedCount + ExtraLength];
            }

        protected:
            SIZE_T        Count;
            SIZE_T        AllocatedCount;

            // +ExtraLength this is a hack method for saving string on it.
            T             StackVal[DEFAULT_LENGTH + ExtraLength];

            T*            HeapValPtr;
        };

        // String Wrapper
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

            TAutoString()
            {
            }

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

            const TCharType* CStr() const
            {
                return GetDataPtr();
            }

            // is is a internal function
            // 
            void InjectAdd(SIZE_T count)
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

        inline size_t LengthOf(const wchar_t* pstr)
        {
            return wcslen(pstr);
        }

        inline size_t LengthOf(const char* pstr)
        {
            return strlen(pstr);
        }

        inline size_t LengthOf(const std::basic_string<char>& str)
        {
            return str.length();
        }

        inline size_t LengthOf(const std::basic_string<wchar_t>& str)
        {
            return str.length();
        }

        inline const wchar_t* PtrOf(const wchar_t* pstr)
        {
            return pstr;
        }

        inline const char* PtrOf(const char* pstr)
        {
            return pstr;
        }

        inline const char* PtrOf(const std::basic_string<char>& str)
        {
            return str.c_str();
        }

        inline const wchar_t* PtrOf(const std::basic_string<wchar_t>& str)
        {
            return str.c_str();
        }
    }
}
