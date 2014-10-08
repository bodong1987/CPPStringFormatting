// FormatLibrary 
// @author bodong 
// Parse Pattern
#pragma once

namespace FormatLibrary
{
    namespace Algorithm
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

            bool operator ()(const CharType* const FormatStart, const SizeType Length, PatternListType& Patterns)
            {
                return ParsePatterns(FormatStart, Length, Patterns);
            }

        protected:
            enum EParseState
            {
                EPS_Literal   = 0x0000000f,
                EPS_OpenCurly = 0x000000f0,
                EPS_Parameter = 0x00000f00,
                EPS_Width     = 0x0000f000,
                EPS_Precision = 0x000f0000
            };

            static INT  CastToSmallNumber(const CharType* const Start, const CharType* const End)
            {
                assert(Start && End && Start < End && "invalid parameters!");
                assert(End - Start < 3 && "too large interger!!!");

                if (!Start || !End || Start >= End)
                {
                    return -1;
                }

                INT Value = 0;

                const CharType* TestPtr = Start;

                while (TestPtr < End)
                {
                    Value = Value * 10 + *Start - '0';

                    ++TestPtr;
                }

                assert(Value < 256 && "paramter index is too large!");

                return Value;
            }

            // only positive number
            static INT FindNextNumber(const CharType* const Start, const CharType* const End, const CharType* & EndPoint)
            {
                const CharType* TestPtr = Start;

                while (TestPtr < End && isdigit(*TestPtr))
                {
                    ++TestPtr;
                }

                if (Start == TestPtr)
                {
                    return -1;
                }

                INT val = CastToSmallNumber(Start, TestPtr);

                EndPoint = TestPtr;

                return val;
            }

            static bool ParseAlignMode(const CharType* const Start, const CharType* const End, FormatPattern& Pattern)
            {
                assert(Start && End && Start < End && "invalid parameters!");

                const CharType* TestPtr = Start;

                ++TestPtr;

                if (TestPtr >= End ||
                    (!isdigit(*TestPtr) && *TestPtr != '-')
                    )
                {
                    return false;
                }

                if (*TestPtr == '-')
                {
                    Pattern.Align = FormatPattern::AF_Left;

                    ++TestPtr;
                }

                if (TestPtr >= End)
                {
                    return true;
                }

                INT val = FindNextNumber(TestPtr, End, TestPtr);

                if (val == -1)
                {
                    return false;
                }

                Pattern.Width = (ByteType)val;

                // find if it is a ':' here
                if (TestPtr < End && *TestPtr == ':')
                {
                    if (!ParseFormatMode(TestPtr, End, Pattern))
                    {
                        return false;
                    }
                }

                return true;
            }

            static bool ParseFormatMode(const CharType* const Start, const CharType* const End, FormatPattern& Pattern)
            {
                assert(Start && End && Start < End && "invalid parameters!");

                const CharType* TestPtr = Start;

                ++TestPtr;

                if (TestPtr >= End)
                {
                    return false;
                }

                switch (toupper(*TestPtr))
                {
                case 'D':
                    Pattern.Flag = FormatPattern::FF_Decimal;
                    break;
                case 'E':
                    Pattern.Flag = FormatPattern::FF_Exponent;
                    break;
                case 'F':
                    Pattern.Flag = FormatPattern::FF_FixedPoint;
                    break;
                case 'P':
                    Pattern.Flag = FormatPattern::FF_Percentage;
                    break;
                case 'X':
                    Pattern.Flag = FormatPattern::FF_Hex;
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

                if (TestPtr >= End)
                {
                    return true;
                }

                if (isdigit(*TestPtr))
                {
                    // try get Precision
                    INT val = FindNextNumber(TestPtr, End, TestPtr);

                    if (val != (ByteType)-1)
                    {
                        Pattern.Precision = (ByteType)val;
                    }
                }

                return true;
            }

            static bool ParseParameter(const CharType* const Start, const CharType* const End, FormatPattern& Parttern)
            {
                // 1. find the parameter index
                assert(Start && End && "invalid parameters!!!");

                const CharType* TestPtr = Start;

                while (TestPtr < End && !isdigit(*TestPtr))
                {
                    ++TestPtr;
                }

                const CharType* TestPtr2 = TestPtr;

                while (TestPtr2 < End && isdigit(*TestPtr2))
                {
                    ++TestPtr2;
                }

                if (TestPtr == TestPtr2)
                {
                    // no parameter.
                    return false;
                }

                INT idx = CastToSmallNumber(TestPtr, TestPtr2);

                if (idx < 0)
                {
                    return false;
                }

                // cast this string to parameter.
                Parttern.Index = static_cast<ByteType>(idx);
                Parttern.Flag = FormatPattern::FF_None;

                TestPtr = TestPtr2;

                if (TestPtr >= End)
                {
                    return true;
                }

                if (*TestPtr == ',')
                {
                    // let's find a align mode and width
                    if (!ParseAlignMode(TestPtr, End, Parttern))
                    {
                        return false;
                    }
                }
                else if (*TestPtr == ':')
                {
                    // let's find a format string
                    if (!ParseFormatMode(TestPtr, End, Parttern))
                    {
                        return false;
                    }
                }

                return true;
            }

            void OnLiteral(
                const CharType*& /*p0*/,
                const CharType*& p1,
                const CharType* const /*Start*/,
                const CharType* const /*End*/,
                EParseState& State,
                PatternListType& /*Patterns*/
                )
            {
                switch (*p1)
                {
                case '{':
                    State = EPS_OpenCurly;
                    break;

                default:
                    break;
                }
            }

            void OnOpenCurly(
                const CharType*& p0,
                const CharType*& p1,
                const CharType* const Start,
                const CharType* const /*End*/,
                EParseState& State,
                PatternListType& Patterns
                )
            {
                if (p1 != p0 + 1)
                {
                    // create a pattern if this is the first slice
                    FormatPattern Pattern;

                    if (p0 == Start)
                    {
                        Pattern.Flag = FormatPattern::FF_Raw;
                        Pattern.Start = 0;
                        Pattern.Len = (SizeType)(p1 - p0) - 1;
                        Pattern.Index = (ByteType)-1;
                    }
                    else
                    {
                        Pattern.Flag = FormatPattern::FF_Raw;
                        Pattern.Start = p0 - Start;
                        Pattern.Len = (SizeType)(p1 - p0) - 1;
                        Pattern.Index = (ByteType)-1;
                    }

                    TPolicy::AppendPattern(Patterns, Pattern);

                    p0 = p1 - 1;
                }

                switch (*p1)
                {
                case '{':
                    {
                        FormatPattern Pattern;

                        Pattern.Flag = FormatPattern::FF_Raw;
                        Pattern.Start = p1 - Start;
                        Pattern.Len = 1;

                        TPolicy::AppendPattern(Patterns, Pattern);

                        p0 = p1 + 1;

                        State = EPS_Literal;
                        break;
                    }
                default:
                    State = EPS_Parameter;
                    break;
                }
            }

            void OnParameter(
                const CharType*& p0,
                const CharType*& p1,
                const CharType* const Start,
                const CharType* const /*End*/,
                EParseState& State,
                PatternListType& Patterns
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
                        State = EPS_Literal;

                        FormatPattern Pattern;

                        if (ParseParameter(p0, p1, Pattern))
                        {
                            Pattern.Start = p0 - Start;
                            Pattern.Len = p1 - p0 + 1;

                            TPolicy::AppendPattern(Patterns, Pattern);
                        }
                        else
                        {
                            FormatPattern rawpatt;

                            rawpatt.Flag = FormatPattern::FF_Raw;
                            rawpatt.Start = p0 - Start;
                            rawpatt.Len = p1 - p0 + 1;

                            TPolicy::AppendPattern(Patterns, Pattern);
                        }

                        p0 = p1 + 1;
                        break;
                    }
               }
            }

            bool ParsePatterns(
                const CharType* const FormatStart, 
                const SizeType Length, 
                PatternListType& Patterns
                )
            {
                const CharType* p0                  = FormatStart;
                const CharType* p1                  = p0;
                const CharType* const Start         = p0;
                const CharType* const End           = FormatStart + Length;
                EParseState     State               = EPS_Literal;

                for( ; p1 != End; ++p1 )
                {
                    switch (State)
                    {
                    case EPS_Literal:
                        OnLiteral(p0, p1, Start, End, State, Patterns);
                        break;
                    case EPS_OpenCurly:
                        OnOpenCurly(p0, p1, Start, End, State, Patterns);
                        break;
                    case EPS_Parameter:
                        OnParameter(p0, p1, Start, End, State, Patterns);
                        break;
                    default:
                        assert( false && "unsupported state!" );
                        break;
                    }
                }

                if( p0 != p1 )
                {
                    FormatPattern RawPattern;

                    RawPattern.Flag  = FormatPattern::FF_Raw;
                    RawPattern.Start = p0-Start;
                    RawPattern.Len   = p1-p0;

                    TPolicy::AppendPattern(Patterns, RawPattern);
                }

                return true;
            }
        };
    }
}