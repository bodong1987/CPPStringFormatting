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
            inline OutputType MoveArrayImpl(InputType Start, InputType End, OutputType Dest, Mpl::FalseType)
            {
                while (Start != End)
                {
                    *Dest = FL_MOVE_SEMANTIC(*Start);
                    ++Dest;
                    ++Start;
                }

                return Dest;
            }

            template <typename T >
            inline T* MoveArrayImpl(T* Start, T* End, T* Dest, Mpl::TrueType)
            {
                const size_t Size = (End - Start)*sizeof(T);

                memcpy(Dest, Start, Size);

                return Dest + (End - Start);
            }

            template <typename InputType, typename OutputType >
            inline OutputType CopyArrayImpl(InputType Start, InputType End, OutputType Dest, Mpl::FalseType)
            {
                while (Start != End)
                {
                    *Dest = *Start;
                    ++Dest;
                    ++Start;
                }

                return Dest;
            }

            template <typename T >
            inline T* CopyArrayImpl(T* Start, T* End, T* Dest, Mpl::TrueType)
            {
                const size_t Size = (End - Start)*sizeof(T);

                memcpy(Dest, Start, Size);

                return Dest + (End - Start);
            }
        }

        template<typename InputType, typename OutputType>
        inline OutputType MoveArray(InputType Start, InputType End, OutputType Dest)
        {
            typedef typename Mpl::TIfElse<
                Mpl::IsSimple<InputType>::Value &&
                Mpl::IsSame<InputType, OutputType>::Value,
                Mpl::TrueType,
                Mpl::FalseType >::Type  Type;

            return Detail::MoveArrayImpl(Start, End, Dest, Type());
        }

        template<typename InputType, typename OutputType>
        inline OutputType CopyArray(InputType Start, InputType End, OutputType Dest)
        {
            typedef typename Mpl::TIfElse<
                Mpl::IsSimple<InputType>::Value &&
                Mpl::IsSame<InputType, OutputType>::Value,
                Mpl::TrueType,
                Mpl::FalseType >::Type  Type;

            return Detail::CopyArrayImpl(Start, End, Dest, Type());
        }

        template< class T > inline T Clamp(const T X, const T Min, const T Max)
        {
            return X < Min ? Min : X < Max ? X : Max;
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
            
            TScopedLocker(TReferenceType& ReferenceVal) :
                Reference(&ReferenceVal)
            {
                assert(Reference);
                Reference->Lock();
            }

            TScopedLocker(TReferenceType* pInReference) :
                Reference(pInReference)
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
                ConstIterator(const SelfType& InRef) :
                    Ref(InRef),
                    Index( InRef.GetLength()>0?0:-1 )
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
            private:
                ConstIterator& operator = (const ConstIterator&);
                ConstIterator(ConstIterator&);
            protected:
                const SelfType&   Ref;
                SIZE_T            Index;
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

            TAutoArray(const SelfType& Other) :
                Count(Other.Count),
                AllocatedCount(Other.AllocatedCount),
                HeapValPtr(NULL)
            {
                if (Count > 0)
                {
                    if (Other.IsDataOnStack())
                    {
                        Algorithm::CopyArray(Other.StackVal, Other.StackVal + Count, StackVal);
                    }
                    else
                    {
                        HeapValPtr = Allocate(AllocatedCount);
                        Algorithm::CopyArray(Other.HeapValPtr, Other.HeapValPtr + Count, HeapValPtr);
                    }
                }
            }

            SelfType& operator = (const SelfType& Other)
            {
                if (this == &Other)
                {
                    return *this;
                }

                ReleaseHeapData();

                Count = Other.Count;
                AllocatedCount = Other.AllocatedCount;
                HeapValPtr = NULL;

                if (Count > 0)
                {
                    if (Other.IsDataOnStack())
                    {
                        Algorithm::CopyArray(Other.StackVal, Other.StackVal + Count, StackVal);
                    }
                    else
                    {
                        HeapValPtr = Allocate(AllocatedCount);
                        Algorithm::CopyArray(Other.HeapValPtr, Other.HeapValPtr + Count, HeapValPtr);
                    }
                }

                return *this;
            }

            SelfType& TakeFrom(SelfType& Other)
            {
                if (this == &Other)
                {
                    return *this;
                }

                Count = Other.Count;
                AllocatedCount = Other.AllocatedCount;
                HeapValPtr = Other.HeapValPtr;

                if (Count > 0 && Other.IsDataOnStack())
                {
                    Algorithm::MoveArray(Other.StackVal, Other.StackVal + Count, StackVal);
                }

                Other.Count = 0;
                Other.AllocatedCount = 0;
                Other.HeapValPtr = NULL;
            }

            void TakeTo(SelfType& Other)
            {
                Other.TakeFrom(*this);
            }

#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
            TAutoArray( SelfType && Other ) :
                Count(Other.Count),
                AllocatedCount(Other.AllocatedCount),
                HeapValPtr(Other.HeapValPtr)
            {
                if (Count > 0 && Other.IsDataOnStack())
                {
                    Algorithm::MoveArray(Other.StackVal, Other.StackVal + Count, StackVal);
                }

                Other.Count = 0;
                Other.AllocatedCount = 0;
                Other.HeapValPtr = NULL;
            }

            SelfType& operator = (SelfType&& Other )
            {
                return TakeFrom(Other);
            }
#endif

            bool  IsDataOnStack() const
            {
                return HeapValPtr == NULL;
            }

            void  AddItem(const T& InValue)
            {
                if (IsDataOnStack())
                {
                    if (Count < DEFAULT_LENGTH)
                    {
                        StackVal[Count] = InValue;
                        ++Count;
                    }
                    else if (Count == DEFAULT_LENGTH)
                    {
                        InitialMoveDataToHeap();

                        assert(Count < AllocatedCount);

                        HeapValPtr[Count] = InValue;
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
                        HeapValPtr[Count] = InValue;
                        ++Count;
                    }
                    else
                    {
                        ExpandHeapSpace();

                        assert(Count < AllocatedCount);
                        HeapValPtr[Count] = InValue;
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
            
            T& operator []( SIZE_T Index )
            {
                assert( Index < GetLength() );
                
                return GetDataPtr()[Index];
            }
            
            const T& operator []( SIZE_T Index ) const
            {
                assert( Index < GetLength() );
                
                return GetDataPtr()[Index];
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

            TAutoString(const CharType* pszStr)
            {
                if (pszStr)
                {
                    const SIZE_T Length = CharTraits::length(pszStr);

                    Count = Length;

                    if (Length <= DEFAULT_LENGTH)
                    {
                        CharTraits::copy(pszStr, pszStr + Length, StackVal);
                        StackVal[Count] = 0;
                    }
                    else
                    {
                        HeapValPtr = Allocate(Length);
                        CharTraits::copy(pszStr, pszStr + Length, HeapValPtr);
                        HeapValPtr[Count] = 0;
                    }
                }
            }

            void  AddChar(CharType InValue)
            {
                AddItem(InValue);

                if (IsDataOnStack())
                {
                    StackVal[Count] = 0;
                }
                else
                {
                    HeapValPtr[Count] = 0;
                }
            }

            void AddStr(const CharType* pszStart, const CharType* pszEnd = NULL)
            {
                const SIZE_T Length = pszEnd ? pszEnd - pszStart : CharTraits::length(pszStart);

                if (IsDataOnStack())
                {
                    if (Count + Length <= DEFAULT_LENGTH)
                    {
                        CharTraits::copy(StackVal+Count, pszStart, Length);
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

                        CharTraits::copy(HeapValPtr+Count, pszStart, Length);

                        Count += Length;

                        HeapValPtr[Count] = 0;
                    }
                }
                else
                {
                    if (Count + Length <= AllocatedCount)
                    {
                        CharTraits::copy(HeapValPtr+Count, pszStart, Length);
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

                        CharTraits::copy(DataPtr, pszStart, Length);

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
            void InjectAdd(SIZE_T InCount)
            {
                Count += InCount;

                assert(IsDataOnStack() ? (Count <= DEFAULT_LENGTH) : (Count < AllocatedCount));
            }

        protected:
            void  AddItem(const TCharType& InValue)
            {
                Super::AddItem(InValue);
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
