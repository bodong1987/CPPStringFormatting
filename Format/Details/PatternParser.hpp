#pragma once

#include <cassert>

#include <Format/Details/Pattern.hpp>

namespace FormatLibrary
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

            bool operator ()(const CharType* const fomatStart, const SizeType length, PatternListType& patterns)
            {
                return ParsePatterns(fomatStart, length, patterns);
            }

        protected:
            enum class EParseState
            {
                EPS_Literal   = 0x0000000F,                
                EPS_OpenCurly = 0x000000F0,                
                EPS_Parameter = 0x00000F00,                
                EPS_Width     = 0x0000F000,                
                EPS_Precision = 0x000F0000
            };

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
                    Value = Value * 10 + *start - '0';

                    ++TestPtr;
                }

                assert(Value < 256 && "parameter index is too large!");

                return Value;
            }

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

                int32_t val = CastToSmallNumber(start, TestPtr);

                endPoint = TestPtr;

                return val;
            }

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

                pattern.Width = (ByteType)val;

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

            static bool ParseFormatMode(const CharType* const start, const CharType* const end, FormatPattern& pattern)
            {
                assert(start && end && start < end && "invalid parameters!");

                const CharType* TestPtr = start;

                ++TestPtr;

                if (TestPtr >= end)
                {
                    return false;
                }

                switch (toupper(*TestPtr))
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
                case 'C':
                case 'R':
                case 'G':
                    // use default.
                    break;
                default:
                    // unsupported Flag!!!
                    return false;
                }

                ++TestPtr;

                if (TestPtr >= end)
                {
                    return true;
                }

                if (TCharTraits<CharType>::IsDigit(*TestPtr))
                {
                    // try get Precision
                    int32_t val = FindNextNumber(TestPtr, end, TestPtr);

                    if (val != (ByteType)-1)
                    {
                        pattern.Precision = (ByteType)val;
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
            /// <param name="">The .</param>
            /// <param name="p1">The p1.</param>
            /// <param name="">The .</param>
            /// <param name="">The .</param>
            /// <param name="state">The state.</param>
            /// <param name="">The .</param>
            void OnLiteral(
                const CharType*& /*p0*/,
                const CharType*& p1,
                const CharType* const /*start*/,
                const CharType* const /*end*/,
                EParseState& state,
                PatternListType& /*patterns*/
            )
            {
                switch (*p1)
                {
                case '{':
                    state = EParseState::EPS_OpenCurly;
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
            /// <param name="">The .</param>
            /// <param name="state">The state.</param>
            /// <param name="patterns">The patterns.</param>
            void OnOpenCurly(
                const CharType*& p0,
                const CharType*& p1,
                const CharType* const start,
                const CharType* const /*end*/,
                EParseState& state,
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
                        pattern.Len = (SizeType)(p1 - p0) - 1;
                        pattern.Index = (ByteType)-1;
                    }
                    else
                    {
                        pattern.Flag = EFormatFlag::Raw;
                        pattern.Start = p0 - start;
                        pattern.Len = (SizeType)(p1 - p0) - 1;
                        pattern.Index = (ByteType)-1;
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

                    state = EParseState::EPS_Literal;
                    break;
                }
                default:
                    state = EParseState::EPS_Parameter;
                    break;
                }
            }

            /// <summary>
            /// Called when get a [parameter].
            /// </summary>
            /// <param name="p0">The p0.</param>
            /// <param name="p1">The p1.</param>
            /// <param name="start">The start.</param>
            /// <param name="">The .</param>
            /// <param name="state">The state.</param>
            /// <param name="patterns">The patterns.</param>
            void OnParameter(
                const CharType*& p0,
                const CharType*& p1,
                const CharType* const start,
                const CharType* const /*end*/,
                EParseState& state,
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
                    state = EParseState::EPS_Literal;

                    FormatPattern pattern;

                    if (ParseParameter(p0, p1, pattern))
                    {
                        pattern.Start = p0 - start;
                        pattern.Len = p1 - p0 + 1;

                        TPolicy::AppendPattern(patterns, pattern);
                    }
                    else
                    {
                        FormatPattern rawpatt;

                        rawpatt.Flag = EFormatFlag::Raw;
                        rawpatt.Start = p0 - start;
                        rawpatt.Len = p1 - p0 + 1;

                        TPolicy::AppendPattern(patterns, pattern);
                    }

                    p0 = p1 + 1;
                    break;
                }
                }
            }

            /// <summary>
            /// Parses the patterns.
            /// </summary>
            /// <param name="fomatStart">The fomat start.</param>
            /// <param name="length">The length.</param>
            /// <param name="patterns">The patterns.</param>
            /// <returns>bool.</returns>
            bool ParsePatterns(
                const CharType* const fomatStart,
                const SizeType length,
                PatternListType& patterns
            )
            {
                const CharType* p0 = fomatStart;
                const CharType* p1 = p0;
                const CharType* const start = p0;
                const CharType* const end = fomatStart + length;
                EParseState     state = EParseState::EPS_Literal;

                for (; p1 != end; ++p1)
                {
                    switch (state)
                    {
                    case EParseState::EPS_Literal:
                        OnLiteral(p0, p1, start, end, state, patterns);
                        break;
                    case EParseState::EPS_OpenCurly:
                        OnOpenCurly(p0, p1, start, end, state, patterns);
                        break;
                    case EParseState::EPS_Parameter:
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

