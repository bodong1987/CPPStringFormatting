#pragma once

#include <Format/Details/Types.hpp>

namespace FormatLibrary
{
    namespace Utility
    {
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
                    index(referenceTarget.GetLength() > 0 ? 0 : -1)
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
                assert(index < GetLength());

                return GetDataPtr()[index];
            }

            /// <summary>
            /// Operator[]s the specified index.
            /// </summary>
            /// <param name="index">The index.</param>
            /// <returns>const T &.</returns>
            const T& operator [](SIZE_T index) const
            {
                assert(index < GetLength());

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
    }
}
