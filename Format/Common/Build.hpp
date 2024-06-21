#pragma once

#if (defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WINDOWS) || defined(_WIN64))
#define FL_PLATFORM_WINDOWS   1
#else
#define FL_PLATFORM_WINDOWS   0
#endif

// on apple platform ??? macos or ios ...
#if defined(__APPLE__) || defined(APPLE) || defined(_APPLE_) || defined(PLATFORM_APPLE)
#define FL_PLATFORM_APPLE     1
#include <TargetConditionals.h>
#else
#define FL_PLATFORM_APPLE     0
#endif

#if defined(__OBJC__)
#define FL_OBJECTIVE_C        1
#else
#define FL_OBJECTIVE_C        0
#endif

#ifdef _MSC_VER
#define FL_COMPILER_MSVC      1
#else
#define FL_COMPILER_MSVC      0
#endif

#ifdef __GNUC__
#define FL_COMPILER_GCC       1
#else
#define FL_COMPILER_GCC       0
#endif // __GNUC__

// ios or ios simulator
#if defined(TARGET_OS_IOS) || defined(TARGET_IPHONE_SIMULATOR)
#define FL_PLATFORM_IOS       1
#else
#define FL_PLATFORM_IOS       0
#endif

// iphone simulator ???
#if defined(TARGET_IPHONE_SIMULATOR)
#define FL_PLATFORM_IPHONE_SIMULATOR 1
#else
#define FL_PLATFORM_IPHONE_SIMULATOR 0
#endif

#if defined(__APPLE__) || defined(APPLE) || defined(_APPLE_)
#define FL_PLATFORM_MACOS     1
#else
#define FL_PLATFORM_MACOS     0
#endif

#if defined(ANDROID) || defined(__ANDROID__)
#define FL_PLATFORM_ANDROID   1
#else
#define FL_PLATFORM_ANDROID   0
#endif


#if defined(_WIN64) || defined(_X64) || defined(WIN64) || defined( __LP64__ )
#define FL_PLATFORM_X64       1
#elif defined(WIN32) || defined(_WIN32)
#define FL_PLATFORM_X64       0
#endif

#if defined(__arm__) || defined(__thumb__) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__arm)
#define FL_PLATFORM_ARM       1
#else
#define FL_PLATFORM_ARM       0
#endif

#if defined(DEBUG)||defined(_DEBUG)
#define FL_DEBUG              1
#else
#define FL_DEBUG              0
#endif

#if defined(UNICODE)||defined(_UNICODE)
#define FL_UNICODE            1
#else
#define FL_UNICODE            0
#endif

// is C++/CLI enabled???
#if FL_PLATFORM_WINDOWS && FL_COMPILER_MSVC && (defined(_MANAGED)||defined(_M_CEE))
#define FL_COMPILER_MSVC_WITH_CPP_CLI 1
#else
#define FL_COMPILER_MSVC_WITH_CPP_CLI 0
#endif

// disable warning for C++/CLI
#if FL_COMPILER_MSVC_WITH_CPP_CLI
#pragma warning(disable : 4793)
#endif

#if ( FL_PLATFORM_WINDOWS && defined(_MSC_VER) && _MSC_VER>=1600 ) || \
    ( FL_COMPILER_GCC && defined(__GXX_EXPERIMENTAL_CXX0X__) ) || \
    FL_PLATFORM_MACOS 
#else
#error "This code requires a C++11 compliant compiler"
#endif

#ifndef _countof
#define _countof( Array ) (sizeof(Array)/sizeof(Array[0]))
#endif

