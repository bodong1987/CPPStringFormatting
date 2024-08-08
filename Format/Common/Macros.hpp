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

// Comma
#define FL_PP_COMMA() ,

// Text
#define _FL_PP_TEXT(Expr) #Expr /*NOLINT*/
#define FL_PP_TEXT(Expr) _FL_PP_TEXT(Expr)

// CAT
#define _FL_PP_CAT_IMPL_(a, b ) a ## b /*NOLINT*/
#define FL_PP_CAT(a, b) _FL_PP_CAT_IMPL_( a, b )

// make a unique name
#define FL_PP_UNIQUE_NAME( Prefix ) \
    FL_PP_CAT(Prefix, FL_PP_CAT(_, __LINE__))

// support not
// ReSharper disable once CommentTypo
// NOLINTBEGIN
#define FL_PP_COMPL_0 1
#define FL_PP_COMPL_1 0
#define _FL_PP_NOT(Expr) FL_PP_CAT(FL_PP_COMPL_, FL_PP_BOOL(Expr))
#define FL_PP_NOT(Expr) _FL_PP_NOT(Expr)
// ReSharper disable once CommentTypo
// NOLINTEND

// Bool
#define FL_PP_BOOL(x) FL_PP_BOOL_I(x)

#define FL_PP_BOOL_I(x) FL_PP_BOOL_ ## x

// ReSharper disable once CommentTypo
// NOLINTBEGIN
#define FL_PP_BOOL_0 0
#define FL_PP_BOOL_1 1
#define FL_PP_BOOL_2 1
#define FL_PP_BOOL_3 1
#define FL_PP_BOOL_4 1
#define FL_PP_BOOL_5 1
#define FL_PP_BOOL_6 1
#define FL_PP_BOOL_7 1
#define FL_PP_BOOL_8 1
#define FL_PP_BOOL_9 1
#define FL_PP_BOOL_10 1
#define FL_PP_BOOL_11 1
#define FL_PP_BOOL_12 1
#define FL_PP_BOOL_13 1
#define FL_PP_BOOL_14 1
#define FL_PP_BOOL_15 1
#define FL_PP_BOOL_16 1
// ReSharper disable once CommentTypo
// NOLINTEND

// Simple If
#define FL_PP_IF(Condition, t, f) FL_PP_IIF(FL_PP_BOOL(Condition), t, f)
#define FL_PP_IIF(Bit, t, f) FL_PP_IIF_I(Bit, t, f)
#define FL_PP_IIF_I(Bit, t, f) FL_PP_IIF_ ## Bit(t, f)
#define FL_PP_IIF_0(t, f) f
#define FL_PP_IIF_1(t, f) t


// Comma
#define FL_PP_COMMA() ,
#define FL_PP_EMPTY()
#define FL_PP_COMMA_IF(Condition) FL_PP_IF(Condition, FL_PP_COMMA, FL_PP_EMPTY)()

// Repeat Semantic
#define FL_PP_REPEAT(n, m, d) FL_PP_CAT(FL_PP_REPEAT_, n)(m, d)

// max iterate count 16
// if you want more, you can add it yourself
#define FL_PP_REPEAT_0(m, d)
#define FL_PP_REPEAT_1(m, d) m(d, 0)
#define FL_PP_REPEAT_2(m, d) FL_PP_REPEAT_1(m, d) m(d, 1)
#define FL_PP_REPEAT_3(m, d) FL_PP_REPEAT_2(m, d) m(d, 2)
#define FL_PP_REPEAT_4(m, d) FL_PP_REPEAT_3(m, d) m(d, 3)
#define FL_PP_REPEAT_5(m, d) FL_PP_REPEAT_4(m, d) m(d, 4)
#define FL_PP_REPEAT_6(m, d) FL_PP_REPEAT_5(m, d) m(d, 5)
#define FL_PP_REPEAT_7(m, d) FL_PP_REPEAT_6(m, d) m(d, 6)
#define FL_PP_REPEAT_8(m, d) FL_PP_REPEAT_7(m, d) m(d, 7)
#define FL_PP_REPEAT_9(m, d) FL_PP_REPEAT_8(m, d) m(d, 8)
#define FL_PP_REPEAT_10(m, d) FL_PP_REPEAT_9(m, d) m(d, 9)
#define FL_PP_REPEAT_11(m, d) FL_PP_REPEAT_10(m, d) m(d, 10)
#define FL_PP_REPEAT_12(m, d) FL_PP_REPEAT_11(m, d) m(d, 11)
#define FL_PP_REPEAT_13(m, d) FL_PP_REPEAT_12(m, d) m(d, 12)
#define FL_PP_REPEAT_14(m, d) FL_PP_REPEAT_13(m, d) m(d, 13)
#define FL_PP_REPEAT_15(m, d) FL_PP_REPEAT_14(m, d) m(d, 14)
#define FL_PP_REPEAT_16(m, d) FL_PP_REPEAT_15(m, d) m(d, 15)

// Expand
#define FL_PP_EXPAND(X) X

// enums
#define FL_ENUM_SCALARS( Macro ) \
    Macro(1) \
    Macro(2) \
    Macro(3) \
    Macro(4) \
    Macro(5) \
    Macro(6) \
    Macro(7) \
    Macro(8) \
    Macro(9) \
    Macro(10) \
    Macro(11) \
    Macro(12) \
    Macro(13) \
    Macro(14) \
    Macro(15) \
    Macro(16)

// make some text here
// it used disable warning for gcc
//

