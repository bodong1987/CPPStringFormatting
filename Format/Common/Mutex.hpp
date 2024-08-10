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
#if _WIN32_WINNT > 0x0600
#define FL_WINDOWS_VISTAR_LATER 1
#else
#define FL_WINDOWS_VISTAR_LATER 0
#endif
#endif
#else
// assume your platform support posix thread library
#include <pthread.h>
#endif

// ReSharper disable once CppEnforceNestedNamespacesStyle
namespace Formatting // NOLINT(*-concat-nested-namespaces)
{
    namespace Details
    {
        /// <summary>
        /// Class SharedMutex.
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        class SharedMutex : Noncopyable /*NOLINT*/
        {
        public:
            SharedMutex() // NOLINT(*-pro-type-member-init)
            {
#if FL_PLATFORM_WINDOWS
#if FL_WINDOWS_VISTAR_LATER
                InitializeSRWLock(&SRWLockValue);
#else
                InitializeCriticalSection(&CriticalSectionValue);
                SetCriticalSectionSpinCount(&CriticalSectionValue, 4000);
#endif
#else
                // pthread_mutexattr_t MutexAtt;
                // pthread_mutexattr_init(&MutexAtt);
                // pthread_mutexattr_settype(&MutexAtt, PTHREAD_MUTEX_RECURSIVE);
                // pthread_mutex_init(&Mutex_t, &MutexAtt);

                pthread_rwlock_init(&RWLock, nullptr);
#endif
            }

            ~SharedMutex()
            {
#if FL_PLATFORM_WINDOWS
#if FL_WINDOWS_VISTAR_LATER
#else
                DeleteCriticalSection(&CriticalSectionValue);
#endif
#else
                // pthread_mutex_destroy(&Mutex_t);
                pthread_rwlock_destroy(&RWLock);
#endif
            }

            void LockUnique()
            {
#if FL_PLATFORM_WINDOWS
#if FL_WINDOWS_VISTAR_LATER
                AcquireSRWLockExclusive(&SRWLockValue);
#else
                EnterCriticalSection(&CriticalSectionValue);
#endif
#else
                // pthread_mutex_lock(&Mutex_t);
                pthread_rwlock_wrlock(&RWLock);
#endif
            }

            void UnLockUnique()
            {
#if FL_PLATFORM_WINDOWS
#if FL_WINDOWS_VISTAR_LATER
                ReleaseSRWLockExclusive(&SRWLockValue);
#else
                LeaveCriticalSection(&CriticalSectionValue);
#endif
#else
                // pthread_mutex_unlock(&Mutex_t);
                pthread_rwlock_unlock(&RWLock);
#endif
            }

            void LockShared()
            {
#if FL_PLATFORM_WINDOWS
#if FL_WINDOWS_VISTAR_LATER
                AcquireSRWLockShared(&SRWLockValue);
#else
                EnterCriticalSection(&CriticalSectionValue);
#endif
#else
                // pthread_mutex_lock(&Mutex_t);
                pthread_rwlock_rdlock(&RWLock);
#endif
            }

            void UnLockShared()
            {
#if FL_PLATFORM_WINDOWS
#if FL_WINDOWS_VISTAR_LATER
                ReleaseSRWLockShared(&SRWLockValue);
#else
                LeaveCriticalSection(&CriticalSectionValue);
#endif
#else
                // pthread_mutex_unlock(&Mutex_t);
                pthread_rwlock_unlock(&RWLock);
#endif
            }

        private:
#if FL_PLATFORM_WINDOWS
#if FL_WINDOWS_VISTAR_LATER
            SRWLOCK          SRWLockValue;
#else
            CRITICAL_SECTION CriticalSectionValue;
#endif
#else
            // pthread_mutex_t Mutex_t;
            pthread_rwlock_t RWLock;
#endif
        };

        /// <summary>
        /// Class SharedMutexNone.
        /// no operation mutex
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        class SharedMutexNone : Noncopyable
        {
        public:
            /// <summary>
            /// unique lock this instance
            /// </summary>
            void LockUnique() {} //NOLINT
            /// <summary>
            /// unique unlock this instance
            /// </summary>
            void UnLockUnique() {} //NOLINT
            /// <summary>
            /// lock this instance in shared mode
            /// </summary>
            void LockShared() {} //NOLINT
            /// <summary>
            /// unlock shared
            /// </summary>
            void UnLockShared() {} //NOLINT
        };

        /// <summary>
        /// Class TUniqueLocker.
        /// LockUnique and UnLockUnique with RAII
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        template < typename TReferenceType >
        class TUniqueLocker : /*NOLINT*/
            Noncopyable
        {
        public:
            typedef TReferenceType        ReferenceType;

            /// <summary>
            /// Initializes a new instance of the <see cref="TUniqueLocker"/> class.
            /// </summary>
            /// <param name="referenceTarget">The reference target.</param>
            explicit TUniqueLocker(TReferenceType& referenceTarget) :
                Reference(&referenceTarget)
            {
                assert(Reference);
                Reference->LockUnique();
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="TUniqueLocker"/> class.
            /// </summary>
            /// <param name="referencePointer">The reference pointer.</param>
            explicit TUniqueLocker(TReferenceType* referencePointer) :
                Reference(referencePointer)
            {
                assert(Reference);
                Reference->LockUnique();
            }

            /// <summary>
            /// Finalizes an instance of the <see cref="TUniqueLocker"/> class.
            /// </summary>
            ~TUniqueLocker()
            {
                Reference->UnLockUnique();
            }

        protected:
            TReferenceType* Reference;
        };

        /// <summary>
        /// Class TSharedLocker.
        /// LockShared and UnLockShared with RAII
        /// Implements the <see cref="Noncopyable" />
        /// </summary>
        /// <seealso cref="Noncopyable" />
        template < typename TReferenceType >
        class TSharedLocker : /*NOLINT*/
            Noncopyable
        {
        public:
            typedef TReferenceType        ReferenceType;

            /// <summary>
            /// Initializes a new instance of the <see cref="TSharedLocker"/> class.
            /// </summary>
            /// <param name="referenceTarget">The reference target.</param>
            explicit TSharedLocker(TReferenceType& referenceTarget) :
                Reference(&referenceTarget)
            {
                assert(Reference);
                Reference->LockShared();
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="TSharedLocker"/> class.
            /// </summary>
            /// <param name="referencePointer">The reference pointer.</param>
            explicit TSharedLocker(TReferenceType* referencePointer) :
                Reference(referencePointer)
            {
                assert(Reference);
                Reference->LockShared();
            }

            /// <summary>
            /// Finalizes an instance of the <see cref="TSharedLocker"/> class.
            /// </summary>
            ~TSharedLocker()
            {
                Reference->UnLockShared();
            }

        protected:
            TReferenceType* Reference;
        };

        typedef TUniqueLocker<SharedMutex>          ScopedLocker;
        typedef TSharedLocker<SharedMutex>          SharedLocker;
    }    
}
