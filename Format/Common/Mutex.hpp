#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Common/Noncopyable.hpp>

#if FL_PLATFORM_WINDOWS
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
// assume your platform support pthread library
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#endif

namespace FormatLibrary
{
    namespace Details
    {
        class Mutex : Noncopyable
        {
        public:
            Mutex()
            {
#if FL_PLATFORM_WINDOWS
                ::InitializeCriticalSection(&CriticalSectionValue);
                ::SetCriticalSectionSpinCount(&CriticalSectionValue, 4000);
#else
                pthread_mutexattr_t MutexAtt;

                pthread_mutexattr_init(&MutexAtt);
                pthread_mutexattr_settype(&MutexAtt, PTHREAD_MUTEX_RECURSIVE);
                pthread_mutex_init(&Mutex, &MutexAtt);
#endif
            }

            ~Mutex()
            {
#if FL_PLATFORM_WINDOWS
                ::DeleteCriticalSection(&CriticalSectionValue);
#else
                pthread_mutex_destroy(&Mutex);
#endif
            }

            void Lock()
            {
#if FL_PLATFORM_WINDOWS
                ::EnterCriticalSection(&CriticalSectionValue);
#else
                pthread_mutex_lock(&Mutex);                
#endif
            }

            void UnLock()
            {
#if FL_PLATFORM_WINDOWS
                ::LeaveCriticalSection(&CriticalSectionValue);
#else
                pthread_mutex_unlock(&Mutex);
#endif
            }

        private:
#if FL_PLATFORM_WINDOWS
            /// <summary>
            /// The critical section value
            /// </summary>
            CRITICAL_SECTION CriticalSectionValue;
#else
            /// <summary>
            /// The mutex
            /// </summary>
            pthread_mutex_t Mutex;
#endif
        };

        class MutexNone : Noncopyable
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

        typedef TScopedLocker<Mutex>   ScopedLocker;
    }    
}
