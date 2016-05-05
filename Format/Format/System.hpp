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

namespace FormatLibrary
{
    namespace System
    {
#if FL_PLATFORM_WINDOWS
        class CriticalSection : 
            Utility::Noncopyable
        {
        public:
            CriticalSection()
            {
                ::InitializeCriticalSection(&CriticalSectionValue);
                ::SetCriticalSectionSpinCount(&CriticalSectionValue, 4000);
            }

            ~CriticalSection()
            {
                ::DeleteCriticalSection(&CriticalSectionValue);
            }

            void Lock()
            {
                ::EnterCriticalSection(&CriticalSectionValue);
            }

            void UnLock()
            {
                ::LeaveCriticalSection(&CriticalSectionValue);
            }
        protected:
            CRITICAL_SECTION CriticalSectionValue;
        };
#else
        // assume your platform support pthread library

        class CriticalSection : 
            Utility::Noncopyable
        {
        public:
            CriticalSection()
            {
                pthread_mutexattr_t MutexAtt;

                pthread_mutexattr_init(&MutexAtt);
                pthread_mutexattr_settype(&MutexAtt, PTHREAD_MUTEX_RECURSIVE);
                pthread_mutex_init(&Mutex, &MutexAtt);
            }

            ~CriticalSection()
            {
                pthread_mutex_destroy(&Mutex);
            }

            inline void Lock()
            {
                pthread_mutex_lock(&Mutex);
            }

            inline void UnLock()
            {
                pthread_mutex_unlock(&Mutex);
            }

        protected:
            pthread_mutex_t Mutex;
        };
#endif

        typedef Utility::TScopedLocker<CriticalSection>   ScopedLocker;

        namespace Api
        {
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