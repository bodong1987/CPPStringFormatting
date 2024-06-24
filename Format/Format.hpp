/*
    MIT License

    Copyright (c) 2024 CppFormatLibrary

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

    Project URL: https://github.com/bodong1987/CPPFormatLibrary
*/
/*
* This is a library that uses the C# formatted string style to format C++ strings. 
* It is a type-safe, reorderable formatting library. 
* Not all C# format string format descriptors are perfectly supported, it only supports most common formatting methods.
* see also: 
* @reference https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-numeric-format-strings
*/
#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Common/Macros.hpp>
#include <Format/Common/CharTraits.hpp>

#include <Format/Details/FormatTo.hpp>
