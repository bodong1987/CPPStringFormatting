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

#include <Format/Common/Build.hpp>

namespace Formatting
{
    /// <summary>
    /// Class Noncopyable.
    /// </summary>
    class Noncopyable // NOLINT
    {
    public:
        Noncopyable() {}  // NOLINT      
        ~Noncopyable() {} // NOLINT
#if FL_COMPILER_IS_GREATER_THAN_CXX11    
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator = (const Noncopyable&) = delete;
#else
	private:
		// declare without implementation
		Noncopyable(const Noncopyable&);
		Noncopyable& operator = (const Noncopyable&);
#endif
    };
}
