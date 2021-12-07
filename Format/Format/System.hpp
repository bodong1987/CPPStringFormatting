// FormatLibrary 
// @author bodong 
// system dependencies
#pragma once

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

/// <summary>
/// The FormatLibrary namespace.
/// </summary>
namespace FormatLibrary
{
    /// <summary>
    /// The System namespace.
    /// </summary>
    namespace System
    {
#if FL_PLATFORM_WINDOWS
        /// <summary>
        /// Class CriticalSection.
        /// simple mutex
        /// Implements the <see cref="Utility::Noncopyable" />
        /// </summary>
        /// <seealso cref="Utility::Noncopyable" />
        class CriticalSection :
            Utility::Noncopyable
        {
        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="CriticalSection"/> class.
            /// </summary>
            CriticalSection()
            {
                ::InitializeCriticalSection(&CriticalSectionValue);
                ::SetCriticalSectionSpinCount(&CriticalSectionValue, 4000);
            }

            /// <summary>
            /// Finalizes an instance of the <see cref="CriticalSection"/> class.
            /// </summary>
            ~CriticalSection()
            {
                ::DeleteCriticalSection(&CriticalSectionValue);
            }

            /// <summary>
            /// Locks this instance.
            /// </summary>
            void Lock()
            {
                ::EnterCriticalSection(&CriticalSectionValue);
            }

            /// <summary>
            /// Uns the lock.
            /// </summary>
            void UnLock()
            {
                ::LeaveCriticalSection(&CriticalSectionValue);
            }
        protected:
            /// <summary>
            /// The critical section value
            /// </summary>
            CRITICAL_SECTION CriticalSectionValue;
        };
#else        
        /// <summary>
        /// Class CriticalSection.
        /// assume your platform support pthread library
        /// Implements the <see cref="Utility::Noncopyable" />
        /// </summary>
        /// <seealso cref="Utility::Noncopyable" />
        class CriticalSection :
            Utility::Noncopyable
        {
        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="CriticalSection"/> class.
            /// </summary>
            CriticalSection()
            {
                pthread_mutexattr_t MutexAtt;

                pthread_mutexattr_init(&MutexAtt);
                pthread_mutexattr_settype(&MutexAtt, PTHREAD_MUTEX_RECURSIVE);
                pthread_mutex_init(&Mutex, &MutexAtt);
            }

            /// <summary>
            /// Finalizes an instance of the <see cref="CriticalSection"/> class.
            /// </summary>
            ~CriticalSection()
            {
                pthread_mutex_destroy(&Mutex);
            }

            /// <summary>
            /// Locks this instance.
            /// </summary>
            inline void Lock()
            {
                pthread_mutex_lock(&Mutex);
            }

            /// <summary>
            /// Uns the lock.
            /// </summary>
            inline void UnLock()
            {
                pthread_mutex_unlock(&Mutex);
            }

        protected:
            /// <summary>
            /// The mutex
            /// </summary>
            pthread_mutex_t Mutex;
        };
#endif

        typedef Utility::TScopedLocker<CriticalSection>   ScopedLocker;

        /// <summary>
        /// The Api namespace.
        /// </summary>
        namespace Api
        {
            /// <summary>
            /// Gets the current thread identifier.
            /// </summary>
            /// <returns>SIZE_T.</returns>
            inline SIZE_T GetThreadID()
            {
#if FL_PLATFORM_WINDOWS
                return ::GetCurrentThreadId();

#elif FL_PLATFORM_ANDROID
                return (SIZE_T)pthread_self();
#else
                pid_t tid = (pid_t)syscall(SYS_gettid);

                return (SIZE_T)tid;
#endif
            }
        }
    }
}