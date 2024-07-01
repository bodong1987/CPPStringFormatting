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

#include <Format/Common/Build.hpp>
#include <Format/Common/Noncopyable.hpp>

#if FL_PLATFORM_WINDOWS
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
// assume your platform support posix thread library
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#endif

namespace Formatting
{
    namespace Details
    {
        /// <summary>
        /// Class Mutex.
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        class Mutex : Noncopyable /*NOLINT*/
        {
        public:
            Mutex()
            {
#if FL_PLATFORM_WINDOWS
                InitializeCriticalSection(&CriticalSectionValue);
                SetCriticalSectionSpinCount(&CriticalSectionValue, 4000);
#else
                pthread_mutexattr_t MutexAtt;

                pthread_mutexattr_init(&MutexAtt);
                pthread_mutexattr_settype(&MutexAtt, PTHREAD_MUTEX_RECURSIVE);
                pthread_mutex_init(&Mutex_t, &MutexAtt);
#endif
            }

            ~Mutex()
            {
#if FL_PLATFORM_WINDOWS
                DeleteCriticalSection(&CriticalSectionValue);
#else
                pthread_mutex_destroy(&Mutex_t);
#endif
            }

            void Lock()
            {
#if FL_PLATFORM_WINDOWS
                EnterCriticalSection(&CriticalSectionValue);
#else
                pthread_mutex_lock(&Mutex_t);
#endif
            }

            void UnLock()
            {
#if FL_PLATFORM_WINDOWS
                LeaveCriticalSection(&CriticalSectionValue);
#else
                pthread_mutex_unlock(&Mutex_t);
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
            pthread_mutex_t Mutex_t;
#endif
        };

        /// <summary>
        /// Class MutexNone.
        /// no operation mutex
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        class MutexNone : Noncopyable
        {
        public:
            /// <summary>
            /// Locks this instance.
            /// </summary>
            void Lock() {} //NOLINT
            /// <summary>
            /// Uns the lock.
            /// </summary>
            void UnLock() {} //NOLINT
        };

        /// <summary>
        /// Class TScopedLocker.
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        template < typename TReferenceType >
        class TScopedLocker : /*NOLINT*/
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
