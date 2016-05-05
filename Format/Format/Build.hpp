// FormatLibrary
// @author bodong
// @brief Build Config
#pragma once

#if (defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WINDOWS) || defined(_WIN64))
#define FL_PLATFORM_WINDOWS   1
#else
#define FL_PLATFORM_WINDOWS   0
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

#if defined(DEBUG)||defined(_DEBUG)
#define FL_DEBUG     1
#else
#define FL_DEBUG     0
#endif

#if ( FL_PLATFORM_WINDOWS && defined(_MSC_VER) && _MSC_VER>=1600 ) || \
    ( FL_COMPILER_GCC && defined(__GXX_EXPERIMENTAL_CXX0X__) ) || \
    FL_PLATFORM_MACOS 
#define FL_PLATFORM_HAS_CPP11 1
#else
#define FL_PLATFORM_HAS_CPP11 0
#endif

#if FL_PLATFORM_HAS_CPP11
#define FL_HAS_TR1_CONTAINERS 1
#else
#define FL_HAS_TR1_CONTAINERS 0
#endif

#if FL_PLATFORM_HAS_CPP11
#define FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE 1
#else
#define FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE 0
#endif

#if FL_PLATFORM_HAS_CPP11
#define FL_STATIC_ASSERT( exp, message ) static_assert( exp, message )
#else
#define FL_STATIC_ASSERT( exp, message ) \
    typedef unsigned char _Static_Assert_Error_ ## __LINE__ [ exp?1:-1 ]
#endif

#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
#define FL_MOVE_SEMANTIC( exp ) std::move(exp)
#else
#define FL_MOVE_SEMANTIC( exp ) exp
#endif

#ifndef _countof
#define _countof( Array ) (sizeof(Array)/sizeof(Array[0]))
#endif

#ifndef FL_PLATFORM_SUPPORT_THREAD_LOCAL

#if FL_PLATFORM_MACOS || FL_COMPILER_MSVC
#define FL_PLATFORM_SUPPORT_THREAD_LOCAL 1
#else
#define FL_PLATFORM_SUPPORT_THREAD_LOCAL 0
#endif

#endif

#ifndef FL_WITH_MULTITHREAD
#define FL_WITH_MULTITHREAD 1
#endif

#if FL_PLATFORM_SUPPORT_THREAD_LOCAL && FL_WITH_MULTITHREAD

#define FL_WITH_THREAD_LOCAL 1

#if FL_PLATFORM_MACOS
#define FL_THREAD_LOCAL __thread
#elif FL_COMPILER_MSVC
#define FL_THREAD_LOCAL __declspec(thread)
#else
#define FL_THREAD_LOCAL thread_local
#endif

#else

#define FL_THREAD_LOCAL
#define FL_WITH_THREAD_LOCAL 0

#endif


#include <cassert>
