// FormatLibrary
// @author bodong
// Utility Classes
#pragma once

#include <Format/Details/Types.hpp>
#include <Format/Details/Noncopyable.hpp>

namespace FormatLibrary
{
    namespace Utility
    {
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

        

    }
}
