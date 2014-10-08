// FormatLibrary
// @author bodong
// Types
#pragma once

#if FL_COMPILER_GCC
#include <cstdlib>
#endif

namespace FormatLibrary
{
    typedef int                 INT;
    typedef unsigned int        UINT;
#if FL_COMPILER_GCC
    typedef std::size_t         SIZE_T;
#else
    typedef size_t              SIZE_T;
#endif // FL_COMPILER_GCC

#if FL_PLATFORM_WINDOWS
    typedef __int64             INT64;
    typedef unsigned __int64    UINT64;
#else
    typedef long long int       INT64;
    typedef unsigned long long  UINT64;
#endif
    typedef float               FLOAT;
    typedef double              DOUBLE;
}
