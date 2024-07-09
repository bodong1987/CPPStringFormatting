// ReSharper disable CppClangTidyClangDiagnosticPragmaMessages
#include <Format/Common/Build.hpp>

#if FL_COMPILER_IS_CXX23
#pragma message("C++ 23")
#elif FL_COMPILER_IS_CXX20
#pragma message("C++ 20")
#elif FL_COMPILER_IS_CXX17
#pragma message("C++ 17")
#elif FL_COMPILER_IS_CXX14
#pragma message("C++ 14")
#elif FL_COMPILER_IS_CXX11
#pragma message("C++ 11")
#elif FL_COMPILER_IS_CXX98
#pragma message("C++ 98")
#else
#error "can't find C++ compiler version."
#endif
