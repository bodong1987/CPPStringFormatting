// FormatLibrary
// @author bodong
// just used to test performance
#pragma once

#include <iostream>
#include <ctime>

#if FL_COMPILER_MACOS
#include <mach/mach_time.h>
#endif

/// <summary>
/// The FormatLibrary namespace.
/// </summary>
namespace FormatLibrary
{
    /// <summary>
    /// The Profile namespace.
    /// </summary>
    namespace Profile
    {
        /// <summary>
        /// Class SystemTimer.
        /// </summary>
        class SystemTimer
        {
        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="SystemTimer"/> class.
            /// </summary>
            SystemTimer() :
                CycleSeconds(0),
                StartTime(0)
            {
#if FL_COMPILER_WINDOWS
                LARGE_INTEGER Frequency;
                ::QueryPerformanceFrequency(&Frequency);
                CycleSeconds = 1.0f / Frequency.QuadPart;
#elif FL_COMPILER_MACOS
                mach_timebase_info_data_t Info;
                mach_timebase_info(&Info);

                CycleSeconds = 1e-9 * (DOUBLE)Info.numer / (DOUBLE)Info.denom;
#else

#endif
                Restart();
            }

            /// <summary>
            /// Times the seconds.
            /// </summary>
            /// <returns>FLOAT.</returns>
            FLOAT            TimeSeconds() const
            {
                return static_cast<FLOAT>(CurrentTimeInternal());
            }

            /// <summary>
            /// Elapses this instance.
            /// </summary>
            /// <returns>FLOAT.</returns>
            FLOAT            Elapse() const
            {
                DOUBLE CurTime = CurrentTimeInternal();

                return static_cast<FLOAT>(CurTime - StartTime);
            }

            /// <summary>
            /// Restarts this instance.
            /// </summary>
            void             Restart()
            {
                StartTime = CurrentTimeInternal();
            }

        protected:
            /// <summary>
            /// Currents the time internal.
            /// </summary>
            /// <returns>DOUBLE.</returns>
            DOUBLE           CurrentTimeInternal() const
            {
#if FL_COMPILER_WINDOWS
                LARGE_INTEGER Cycles;
                QueryPerformanceCounter(&Cycles);
                // Add big number to make bugs apparent where return value is being passed to FLOAT
                return Cycles.QuadPart * CycleSeconds/* + 16777216.0*/;

#elif FL_COMPILER_MACOS
                UINT64 Cycles = mach_absolute_time();
                // Add big number to make bugs apparent where return value is being passed to FLOAT
                return Cycles * CycleSeconds/* + 16777216.0*/;
#else
                return static_cast<DOUBLE>(std::clock());
#endif
            }

        protected:
            /// <summary>
            /// The cycle seconds
            /// </summary>
            DOUBLE           CycleSeconds;
            /// <summary>
            /// The start time
            /// </summary>
            DOUBLE           StartTime;
        };

        /// <summary>
        /// Class ProgressTimer.
        /// Implements the <see cref="Utility::Noncopyable" />
        /// </summary>
        /// <seealso cref="Utility::Noncopyable" />
        class ProgressTimer : public Utility::Noncopyable
        {
        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="ProgressTimer"/> class.
            /// </summary>
            /// <param name="TestPath">The test path.</param>
            ProgressTimer(const char* TestPath)
            {
                assert( TestPath );

#if FL_COMPILER_MSVC
                strcpy_s(szTag, TestPath);
#else
                strcpy(szTag, TestPath);
#endif
            }

            /// <summary>
            /// Finalizes an instance of the <see cref="ProgressTimer"/> class.
            /// </summary>
            ~ProgressTimer()
            {
                std::cout << System::Api::GetThreadID()<< " " <<  szTag << "Elapse:" << Timer.Elapse() << std::endl;
            }

        private:
            /// <summary>
            /// The timer
            /// </summary>
            SystemTimer      Timer;
            /// <summary>
            /// The sz tag
            /// </summary>
            char             szTag[128];
        };
    }
}
