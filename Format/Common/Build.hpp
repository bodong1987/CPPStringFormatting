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

// ReSharper disable once CommentTypo
// NOLINTBEGIN
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
#endif // __GNUC__  // NOLINT

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

#if FL_COMPILER_MSVC
// @reference https://www.cnblogs.com/bodong/p/18293350
// Each version of Visual Studio does not have complete support for the C++ standard,
// so simply judging the feature support of the C++ standard through these version numbers is incomplete.
#if _MSC_FULL_VER <= 150030729 // before Visual Studio 2008 sp1, set C++ 98
#define _MSVC_LANG 199711
#elif _MSC_FULL_VER <= 180021114 // before Visual Studio 2013 Nobemver CTP, set C++ 11
#define _MSVC_LANG 201103
#elif _MSC_FULL_VER <= 190023918 // before Visual Studio 2015 Update 2, set C++ 14
#define _MSVC_LANG 201402
#endif // after Visual Studio 2015 Update 3, _MSVC_LANG exists

#define FL_COMPILER_LANG_VERSION _MSVC_LANG
#elif defined(__cplusplus)
#define FL_COMPILER_LANG_VERSION __cplusplus
#else // set C++ 98 as default
#define FL_COMPILER_LANG_VERSION 199711
#pragma message("No valid C++ standard identification flag found, default to C++98 standard")
#endif

// is greater than ?
// Checks whether the current C++ standard is a later version
#define FL_COMPILER_IS_GREATER_THAN_CXX23 (FL_COMPILER_LANG_VERSION >= 202101)
#define FL_COMPILER_IS_GREATER_THAN_CXX20 (FL_COMPILER_LANG_VERSION >= 202002)
#define FL_COMPILER_IS_GREATER_THAN_CXX17 (FL_COMPILER_LANG_VERSION >= 201703)
#define FL_COMPILER_IS_GREATER_THAN_CXX14 (FL_COMPILER_LANG_VERSION >= 201402)
#define FL_COMPILER_IS_GREATER_THAN_CXX11 (FL_COMPILER_LANG_VERSION >= 201103)
#define FL_COMPILER_IS_GREATER_TAHN_CXX98 (FL_COMPILER_LANG_VERSION >= 199711)

// is C++ xx ?
// Check whether the current C++ standard specifies a certain version
#define FL_COMPILER_IS_CXX23 (FL_COMPILER_LANG_VERSION >= 202101)
#define FL_COMPILER_IS_CXX20 (FL_COMPILER_LANG_VERSION >= 202002 && FL_COMPILER_LANG_VERSION < 202101)
#define FL_COMPILER_IS_CXX17 (FL_COMPILER_LANG_VERSION >= 201703 && FL_COMPILER_LANG_VERSION < 202002)
#define FL_COMPILER_IS_CXX14 (FL_COMPILER_LANG_VERSION >= 201402 && FL_COMPILER_LANG_VERSION < 201703)
#define FL_COMPILER_IS_CXX11 (FL_COMPILER_LANG_VERSION >= 201103 && FL_COMPILER_LANG_VERSION < 201402)
#define FL_COMPILER_IS_CXX98 (FL_COMPILER_LANG_VERSION >= 199711 && FL_COMPILER_LANG_VERSION < 201103)

#if FL_COMPILER_IS_GREATER_THAN_CXX11
#define FL_STATIC_ASSERT( exp, message ) static_assert( exp, message )
#define FL_MOVE_SEMANTIC( exp ) std::move(exp)
#else
#define FL_STATIC_ASSERT( exp, message ) \
	typedef unsigned char _Static_Assert_Error_ ## __LINE__ [ exp?1:-1 ]
#define FL_MOVE_SEMANTIC( exp ) exp
#endif

#ifndef FL_COMPILER_SUPPORT_THREAD_LOCAL

#if FL_PLATFORM_MACOS || FL_COMPILER_MSVC
#define FL_COMPILER_SUPPORT_THREAD_LOCAL 1
#else
#define FL_COMPILER_SUPPORT_THREAD_LOCAL 0
#endif

#endif

#ifndef FL_WITH_MULTITHREAD_SUPPORT
#define FL_WITH_MULTITHREAD_SUPPORT 1
#endif

#if FL_COMPILER_SUPPORT_THREAD_LOCAL && FL_WITH_MULTITHREAD_SUPPORT

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

#ifndef FL_ARRAY_COUNTOF
#define FL_ARRAY_COUNTOF( Array ) (sizeof(Array)/sizeof(Array[0])) /*NOLINT*/
#endif

#if FL_COMPILER_IS_GREATER_THAN_CXX11
#include <cstdint>
#define FL_CONSTEXPR constexpr
#else
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef int long long int64_t;
typedef unsigned int long long uint64_t;

#ifndef nullptr
#define nullptr NULL
#endif

#ifndef constexpr
#define constexpr const
#endif

#define FL_CONSTEXPR 
#endif

// ReSharper disable once CommentTypo
// NOLINTEND
