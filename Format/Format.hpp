// FormatLibrary 
// @author bodong 
/*
this simple project is a C++ format library, it use .net style to format string. if you have used C++/CLI or C#, you will be very familiar with this code:

string str;
StandardLibrary::FormatTo(str, "{0}--#--{1,8}--#--{2}", 100, -40.2f, " String ");

and then, str is:
100--#--  -40.20--#-- String

this library is a cross-platform libary, i have compiled it under windows, macos, linux with msvc, g++ and xcode darwin C++ with c++ 11 support or with out c++ 11 support. 
and in most case, it will be 3x fast than c sprintf and it is thread safe.
this project design like boost, it is a header only project. if you want to use it, you only have to include the main Format.hpp.

@github:https://github.com/sczybt/CPPFormatLibrary
*/

// Main Entry
#pragma once

// include build config 
#include "Format/Build.hpp"

// Preprocessor
#include "Format/Preprocessor.hpp"

// include types
#include "Format/Types.hpp"

// include traits classes
#include "Format/Traits.hpp"

// include utility classes
#include "Format/Utility.hpp"

// include system dependencies classes
#include "Format/System.hpp"

// include pattern definitions
#include "Format/Pattern.hpp"

// include default pattern parser
#include "Format/PatternParser.hpp"

// include pattern storage
#include "Format/LookupPattern.hpp"

// include standard library policy
#include "Format/StandardPatternPolicy.hpp"

// include fast string algorithm
#include "Format/Algorithm.hpp"

// include translator implement
#include "Format/Translator.hpp"

// include format to interface
#include "Format/FormatTo.hpp"

// include stl format support
#include "Format/FormatToSTL.hpp"


