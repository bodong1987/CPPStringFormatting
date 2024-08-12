// ReSharper disable CppClangTidyClangDiagnosticPragmaMessages
#include <Format/Common/Build.hpp>

#pragma message(FL_CXX_STANDARD)

#if FL_PLATFORM_X64
#pragma message("64bit platform...")
#else
#pragma message("32bit platform...")
#endif
