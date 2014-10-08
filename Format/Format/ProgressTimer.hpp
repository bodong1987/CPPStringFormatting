// FormatLibrary
// @author bodong
// just used to test performance
#pragma once

#include <iostream>
#include <ctime>

#if FL_PLATFORM_MACOS
#include <mach/mach_time.h>
#endif

namespace FormatLibrary
{
    namespace Profile
    {
        class SystemTimer
        {
        public:
            SystemTimer() :
                CycleSeconds(0),
                StartTime(0)
            {
#if FL_PLATFORM_WINDOWS
                LARGE_INTEGER Frequency;
                ::QueryPerformanceFrequency(&Frequency);
                CycleSeconds = 1.0f / Frequency.QuadPart;
#elif FL_PLATFORM_MACOS
                mach_timebase_info_data_t Info;
                mach_timebase_info(&Info);

                CycleSeconds = 1e-9 * (DOUBLE)Info.numer / (DOUBLE)Info.denom;
#else

#endif
                Restart();
            }

            FLOAT            TimeSeconds() const
            {
                return static_cast<FLOAT>(CurrentTimeInternal());
            }

            FLOAT            Elapse() const
            {
                DOUBLE CurTime = CurrentTimeInternal();

                return static_cast<FLOAT>(CurTime - StartTime);
            }

            void             Restart()
            {
                StartTime = CurrentTimeInternal();
            }

        protected:
            DOUBLE           CurrentTimeInternal() const
            {
#if FL_PLATFORM_WINDOWS
                LARGE_INTEGER Cycles;
                QueryPerformanceCounter(&Cycles);
                // Add big number to make bugs apparent where return value is being passed to FLOAT
                return Cycles.QuadPart * CycleSeconds/* + 16777216.0*/;

#elif FL_PLATFORM_MACOS
                UINT64 Cycles = mach_absolute_time();
                // Add big number to make bugs apparent where return value is being passed to FLOAT
                return Cycles * CycleSeconds/* + 16777216.0*/;
#else
                return static_cast<DOUBLE>(std::clock());
#endif
            }

        protected:
            DOUBLE           CycleSeconds;
            DOUBLE           StartTime;
        };

        class ProgressTimer : public Utility::Noncopyable
        {
        public:
            ProgressTimer( const char* TestPath )
            {
                assert( TestPath );

#if FL_COMPILER_MSVC
                strcpy_s(szTag, TestPath);
#else
                strcpy(szTag, TestPath);
#endif
            }

            ~ProgressTimer()
            {
                std::cout << System::Api::GetThreadID()<< " " <<  szTag << "Elapse:" << Timer.Elapse() << std::endl;
            }

        private:
            SystemTimer      Timer;
            char             szTag[128];
        };
    }
}
