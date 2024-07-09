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

#include <cassert>

#include <Format/Details/Pattern.hpp>

namespace Formatting
{
    namespace Details
    {
        template < typename TPolicy >
        class TPatternParser
        {
        public:
            typedef typename TPolicy::CharType                      CharType;
            typedef typename TPolicy::ByteType                      ByteType;
            typedef typename TPolicy::SizeType                      SizeType;
            typedef typename TPolicy::PatternListType               PatternListType;
            typedef TFormatPattern<CharType>                        FormatPattern;

            bool operator ()(const CharType* const formatStart, const SizeType length, PatternListType& patterns)
            {
                return ParsePatterns(formatStart, length, patterns);
            }

        protected:
#if FL_COMPILER_IS_GREATER_THAN_CXX11
            enum class EParseState : uint8_t
#else
            struct EParseState
            {
                enum Enum
#endif
                {
                    Literal    = 1 << 0,                
                    OpenCurly  = 1 << 1,                
                    CloseCurly = 1 << 2,
                    Parameter  = 1 << 3,                
                    Width      = 1 << 4,                
                    Precision  = 1 << 5
                };

#if FL_COMPILER_IS_GREATER_THAN_CXX11
            typedef EParseState                 ParseStateType;
#else
            };

            typedef typename EParseState::Enum  ParseStateType;
#endif

            /// <summary>
            /// Casts to small number.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <returns>int32_t.</returns>
            static int32_t CastToSmallNumber(const CharType* const start, const CharType* const end)
            {
                assert(start && end && start < end && "invalid parameters!");
                assert(end - start < 3 && "too large integer!!!");

                if (!start || !end || start >= end)
                {
                    return -1;
                }

                int32_t Value = 0;

                const CharType* TestPtr = start;

                while (TestPtr < end)
                {
                    Value = Value * 10 + *TestPtr - '0';

                    ++TestPtr;
                }

                assert(Value < 256 && "parameter index is too large!");

                return Value;
            }

            /// <summary>
            /// Finds the next number.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="endPoint">The end point.</param>
            /// <returns>int32_t.</returns>
            static int32_t FindNextNumber(const CharType* const start, const CharType* const end, const CharType*& endPoint)
            {
                const CharType* TestPtr = start;

                while (TestPtr < end && TCharTraits<CharType>::IsDigit(*TestPtr))
                {
                    ++TestPtr;
                }

                if (start == TestPtr)
                {
                    return -1;
                }

                const int32_t Value = CastToSmallNumber(start, TestPtr);

                endPoint = TestPtr;

                return Value;
            }

            /// <summary>
            /// Parses the align mode.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="pattern">The pattern.</param>
            /// <returns>bool.</returns>
            static bool ParseAlignMode(const CharType* const start, const CharType* const end, FormatPattern& pattern)
            {
                assert(start && end && start < end && "invalid parameters!");

                const CharType* TestPtr = start;

                ++TestPtr;

                if (TestPtr >= end || (!TCharTraits<CharType>::IsDigit(*TestPtr) && *TestPtr != '-'))
                {
                    return false;
                }

                if (*TestPtr == '-')
                {
                    pattern.Align = EAlignFlag::Left;

                    ++TestPtr;
                }

                if (TestPtr >= end)
                {
                    return true;
                }

                int32_t val = FindNextNumber(TestPtr, end, TestPtr);

                if (val == -1)
                {
                    return false;
                }

                pattern.Width = static_cast<ByteType>(val);

                // find if it is a ':' here
                if (TestPtr < end && *TestPtr == ':')
                {
                    if (!ParseFormatMode(TestPtr, end, pattern))
                    {
                        return false;
                    }
                }

                return true;
            }

            /// <summary>
            /// Parses the format mode.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="pattern">The pattern.</param>
            /// <returns>bool.</returns>
            static bool ParseFormatMode(const CharType* const start, const CharType* const end, FormatPattern& pattern)
            {
                assert(start && end && start < end && "invalid parameters!");

                const CharType* TestPtr = start;

                ++TestPtr;

                if (TestPtr >= end)
                {
                    return false;
                }

                switch (TCharTraits<CharType>::ToUpper(*TestPtr))
                {
                case 'D':
                    pattern.Flag = EFormatFlag::Decimal;
                    break;
                case 'E':
                    pattern.Flag = EFormatFlag::Exponent;
                    break;
                case 'F':
                    pattern.Flag = EFormatFlag::FixedPoint;
                    break;
                case 'P':
                    pattern.Flag = EFormatFlag::Percentage;
                    break;
                case 'X':
                    pattern.Flag = EFormatFlag::Hex;
                    break;
                case 'G':
                    pattern.Flag = EFormatFlag::General;
                    break;
                case 'B':
                    pattern.Flag = EFormatFlag::Binary;
                    break;
                case 'C':
                case 'R':                
                    // use default.
                    break;
                default:
                    // unsupported Flag!!!
                    return false;
                }

                pattern.bUpper = TCharTraits<CharType>::IsUpper(*TestPtr);

                ++TestPtr;

                if (TestPtr >= end)
                {
                    return true;
                }

                if (TCharTraits<CharType>::IsDigit(*TestPtr))
                {
                    // get Precision
                    int32_t val = FindNextNumber(TestPtr, end, TestPtr);

                    if (val != static_cast<ByteType>(-1))
                    {
                        pattern.Precision = static_cast<ByteType>(val);
                    }
                }

                return true;
            }

            /// <summary>
            /// Parses the parameter.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="pattern">The pattern.</param>
            /// <returns>bool.</returns>
            static bool ParseParameter(const CharType* const start, const CharType* const end, FormatPattern& pattern)
            {
                // 1. find the parameter index
                assert(start && end && "invalid parameters!!!");

                const CharType* TestPtr = start;

                while (TestPtr < end && !TCharTraits<CharType>::IsDigit(*TestPtr))
                {
                    ++TestPtr;
                }

                const CharType* TestPtr2 = TestPtr;

                while (TestPtr2 < end && TCharTraits<CharType>::IsDigit(*TestPtr2))
                {
                    ++TestPtr2;
                }

                if (TestPtr == TestPtr2)
                {
                    // no parameter.
                    return false;
                }

                int32_t idx = CastToSmallNumber(TestPtr, TestPtr2);

                if (idx < 0)
                {
                    return false;
                }

                // cast this string to parameter.
                pattern.Index = static_cast<ByteType>(idx);
                pattern.Flag = EFormatFlag::None;

                TestPtr = TestPtr2;

                if (TestPtr >= end)
                {
                    return true;
                }

                if (*TestPtr == ',')
                {
                    // let's find a align mode and width
                    if (!ParseAlignMode(TestPtr, end, pattern))
                    {
                        return false;
                    }
                }
                else if (*TestPtr == ':')
                {
                    // let's find a format string
                    if (!ParseFormatMode(TestPtr, end, pattern))
                    {
                        return false;
                    }
                }

                return true;
            }

            /// <summary>
            /// Called when get a [literal].
            /// </summary>            
            void OnLiteral(
                const CharType*& /*p0*/,
                const CharType*& p1,
                const CharType* const /*start*/,
                const CharType* const /*end*/,
                ParseStateType& state,
                PatternListType& /*patterns*/
            )
            {
                switch (*p1)
                {
                case '{':
                    state = EParseState::OpenCurly;
                    break;

                case '}':
                    state = EParseState::CloseCurly;
                    break;

                default:
                    break;
                }
            }

            /// <summary>
            /// Called when get a [open curly].
            /// </summary>
            /// <param name="p0">The p0.</param>
            /// <param name="p1">The p1.</param>
            /// <param name="start">The start.</param>
            /// <param name="state">The state.</param>
            /// <param name="patterns">The patterns.</param>
            void OnOpenCurly(
                const CharType*& p0,
                const CharType*& p1,
                const CharType* const start,
                const CharType* const /*end*/,
                ParseStateType& state,
                PatternListType& patterns
            )
            {
                if (p1 != p0 + 1)
                {
                    // create a pattern if this is the first slice
                    FormatPattern pattern;

                    if (p0 == start)
                    {
                        pattern.Flag = EFormatFlag::Raw;
                        pattern.Start = 0;
                        pattern.Len = static_cast<SizeType>(p1 - p0) - 1;
                        pattern.Index = static_cast<ByteType>(-1);
                    }
                    else
                    {
                        pattern.Flag = EFormatFlag::Raw;
                        pattern.Start = p0 - start;
                        pattern.Len = static_cast<SizeType>(p1 - p0) - 1;
                        pattern.Index = static_cast<ByteType>(-1);
                    }

                    TPolicy::AppendPattern(patterns, pattern);

                    p0 = p1 - 1;
                }

                switch (*p1)
                {
                case '{':
                {
                    FormatPattern pattern;

                    pattern.Flag = EFormatFlag::Raw;
                    pattern.Start = p1 - start;
                    pattern.Len = 1;

                    TPolicy::AppendPattern(patterns, pattern);

                    p0 = p1 + 1;

                    state = EParseState::Literal;
                    break;
                }
                default:
                    state = EParseState::Parameter;
                    break;
                }
            }

            /// <summary>
            /// Called when get a [close curly].
            /// </summary>
            void OnCloseCurly(
                const CharType*& p0,
                const CharType*& p1,
                const CharType* const start,
                const CharType* const /*end*/,
                ParseStateType& state,
                PatternListType& patterns
            )
            {
                if (p1 != p0 + 1)
                {
                    // create a pattern if this is the first slice
                    FormatPattern pattern;

                    if (p0 == start)
                    {
                        pattern.Flag = EFormatFlag::Raw;
                        pattern.Start = 0;
                        pattern.Len = static_cast<SizeType>(p1 - p0) - 1;
                        pattern.Index = static_cast<ByteType>(-1);
                    }
                    else
                    {
                        pattern.Flag = EFormatFlag::Raw;
                        pattern.Start = p0 - start;
                        pattern.Len = static_cast<SizeType>(p1 - p0) - 1;
                        pattern.Index = static_cast<ByteType>(-1);
                    }

                    TPolicy::AppendPattern(patterns, pattern);

                    p0 = p1 + 1;

                    state = EParseState::Literal;
                }

                switch (*p1)
                {
                case '}':
                {
                    FormatPattern pattern;

                    pattern.Flag = EFormatFlag::Raw;
                    pattern.Start = p1 - start;
                    pattern.Len = 1;

                    TPolicy::AppendPattern(patterns, pattern);

                    p0 = p1 + 1;

                    state = EParseState::Literal;
                    break;
                }
                default:
                    break;
                }
            }

            /// <summary>
            /// Called when get a [parameter].
            /// </summary>
            /// <param name="p0">The p0.</param>
            /// <param name="p1">The p1.</param>
            /// <param name="start">The start.</param>
            /// <param name="state">The state.</param>
            /// <param name="patterns">The patterns.</param>
            void OnParameter(
                const CharType*& p0,
                const CharType*& p1,
                const CharType* const start,
                const CharType* const /*end*/,
                ParseStateType& state,
                PatternListType& patterns
            )
            {
                switch (*p1)
                {
                case    '0':
                case    '1':
                case    '2':
                case    '3':
                case    '4':
                case    '5':
                case    '6':
                case    '7':
                case    '8':
                case    '9':
                case    ':':
                case    ',':
                    break;
                case    '}':
                {
                    state = EParseState::Literal;

                    FormatPattern pattern;

                    if (ParseParameter(p0, p1, pattern))
                    {
                        pattern.Start = p0 - start;
                        pattern.Len = p1 - p0 + 1;

                        TPolicy::AppendPattern(patterns, pattern);
                    }
                    else
                    {
                        FormatPattern format_pattern;

                        format_pattern.Flag = EFormatFlag::Raw;
                        format_pattern.Start = p0 - start;
                        format_pattern.Len = p1 - p0 + 1;

                        TPolicy::AppendPattern(patterns, pattern);
                    }

                    p0 = p1 + 1;
                    break;
                }
                default:
                    break;
                }
            }

            /// <summary>
            /// Parses the patterns.
            /// </summary>
            /// <param name="formatStart">The format start.</param>
            /// <param name="length">The length.</param>
            /// <param name="patterns">The patterns.</param>
            /// <returns>bool.</returns>
            bool ParsePatterns(
                const CharType* const formatStart,
                const SizeType length,
                PatternListType& patterns
            )
            {
                const CharType* p0 = formatStart;
                const CharType* p1 = p0;
                const CharType* const start = p0;
                const CharType* const end = formatStart + length;
                ParseStateType     state = EParseState::Literal;

                for (; p1 < end; ++p1)
                {
                    switch (state) // NOLINT(clang-diagnostic-switch-enum)
                    {
                    case EParseState::Literal:
                        OnLiteral(p0, p1, start, end, state, patterns);
                        break;
                    case EParseState::OpenCurly:
                        OnOpenCurly(p0, p1, start, end, state, patterns);
                        break;
                    case EParseState::CloseCurly:
                        OnCloseCurly(p0, p1, start, end, state, patterns);
                        break;
                    case EParseState::Parameter:
                        OnParameter(p0, p1, start, end, state, patterns);
                        break;
                    default: 
                        assert(false && "unsupported state!");
                        break;
                    }
                }

                if (p0 != p1)
                {
                    FormatPattern RawPattern;

                    RawPattern.Flag = EFormatFlag::Raw;
                    RawPattern.Start = p0 - start;
                    RawPattern.Len = p1 - p0;

                    TPolicy::AppendPattern(patterns, RawPattern);
                }

                return true;
            }
        };
    }
}

