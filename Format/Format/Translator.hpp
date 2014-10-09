// FormatLibrary
// @author bodong
// translator
#pragma once

#if FL_COMPILER_MSVC
#include <intsafe.h>
#else
#include <limits.h>
#endif

namespace FormatLibrary
{
    namespace Detail
    {
        template < UINT64 MaxValue >
        struct TMaxLength
        {
            enum
            {
                Value = TMaxLength<MaxValue/10>::Value + 1
            };
        };

        template <>
        struct TMaxLength<0>
        {
            enum
            {
                Value = 1
            };
        };
    }

    template < typename TCharType, typename T >
    class TTranslatorBase
    {
    public:
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;

        template < typename TNumeric >
        static TNumeric Min( TNumeric x, TNumeric y )
        {
            return x < y?x:y;
        }

        template < typename TNumeric >
        static TNumeric Max(TNumeric x, TNumeric y)
        {
            return x < y ? y : x;
        }

        static SIZE_T AdjustString(
            CharType* pszStr,
            SIZE_T Length,
            const SIZE_T BufLength,
            const FormatPattern& Pattern
            )
        {
            SIZE_T UsedLength = Length;

            if (Pattern.HasWidth() && Pattern.Width > Length)
            {
                const SIZE_T PadCount = Min(BufLength - Length, Pattern.Width - Length);

                assert(PadCount < 0xFF);

                if (Pattern.Align == FormatPattern::AF_Left)
                {
                    // Fill at the end
                    CharTraits::Fill(pszStr + Length, ' ', PadCount);

                    pszStr[Length + PadCount] = 0;
                }
                else
                {
                    // fill at the start
                    CharTraits::copy(pszStr + PadCount, pszStr, Length);
                    CharTraits::Fill(pszStr, ' ', PadCount);
                    pszStr[Length + PadCount] = 0;
                }

                UsedLength = Length + PadCount;
            }

            return UsedLength;
        }

        static SIZE_T EstimateLength(const FormatPattern& Pattern, SIZE_T MaxLength)
        {
            if (Pattern.HasWidth())
            {
                return Max(static_cast<SIZE_T>(Pattern.Width), MaxLength);
            }

            return MaxLength;
        }
    };

    template < typename TCharType >
    class TRawTranslator
    {
    public:
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef Utility::TAutoString<CharType>                      StringType;

        // Raw Translator always success
        static bool Transfer(StringType& S, const FormatPattern& Pattern, const CharType* Format)
        {
            assert(Pattern.Len > 0 && "invalid parameters!!!");

            S.AddStr(&Format[Pattern.Start], &Format[Pattern.Start] + Pattern.Len);

            return true;
        }
    };

    template < typename TCharType, typename T >
    class TTranslator : public TTranslatorBase< TCharType, T >
    {
    protected:
        typedef TTranslatorBase< TCharType, T > Super;

        // if you get a compile error with this function can't visit
        // it means that you have transfer an unsupported parameter to Format pipeline
        // you can do them to fix this error:
        //    1. change your code, convert it to the support type
        //    2. make a specialization of TTranslator for your type.
        static bool Transfer(typename Super::StringType& S, const typename Super::FormatPattern& Pattern, const T& Arg);
    };

    template < typename TCharType >
    class TTranslator< TCharType, bool > :
        public TTranslatorBase< TCharType, bool >
    {
    public:
        typedef TTranslatorBase< TCharType, bool >                  Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;

        static bool Transfer(StringType& S, const FormatPattern& Pattern, bool Arg)
        {
            TCharType TrueStr[] = { 'T', 'r', 'u', 'e' };
            TCharType FalseStr[] = { 'F', 'a', 'l', 's', 'e' };

            const TCharType* Selection = Arg ? TrueStr : FalseStr;
            const SIZE_T Length = Arg ? _countof(TrueStr) : _countof(FalseStr);

            S.AddStr(Selection, Selection + Length);

            return true;
        }
    };

    enum
    {
        DEFAULT_FLOAT_PERCISION = 2
    };

    template < typename TCharType >
    class TTranslator< TCharType, DOUBLE > :
    public TTranslatorBase< TCharType, DOUBLE >
    {
    public:
        typedef TTranslatorBase< TCharType, DOUBLE >                Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;

        static bool Transfer(StringType& S, const FormatPattern& Pattern, DOUBLE Arg, SIZE_T UnusedParam = 0 )
        {
            FL_UNREFERENCED_PARAMETER(UnusedParam);

            if (Pattern.Flag == FormatPattern::FF_None ||
                Pattern.Flag == FormatPattern::FF_FixedPoint ||
                Pattern.Flag == FormatPattern::FF_General
                )
            {
                // assume Double value will less than 32
                const SIZE_T EstimatedLength = Super::EstimateLength(Pattern, 32);
                const SIZE_T Capacity = S.GetCapacity();

                if (Capacity >= EstimatedLength)
                {
                    SIZE_T Length = Algorithm::DoubleToString<CharType>(
                        Arg,
                        S.GetUnusedPtr(),
                        Capacity,
                        Pattern.HasPrecision() ? Pattern.Precision : DEFAULT_FLOAT_PERCISION
                        );

                    Length = Super::AdjustString(S.GetUnusedPtr(), Length, Capacity, Pattern);

                    assert(Length <= Capacity);

                    S.InjectAdd(Length);
                }
                else
                {
                    CharType TempBuf[0xFF];

                    SIZE_T Length = Algorithm::DoubleToString<CharType>(
                        Arg,
                        TempBuf,
                        _countof(TempBuf),
                        Pattern.HasPrecision() ? Pattern.Precision : DEFAULT_FLOAT_PERCISION
                        );

                    Length = Super::AdjustString(TempBuf, Length, _countof(TempBuf), Pattern);

                    S.AddStr(TempBuf, TempBuf + Length);
                }

                return true;
            }
            else if (Pattern.Flag == FormatPattern::FF_Decimal)
            {
                return TTranslator<TCharType, INT64>::Transfer(S, Pattern, static_cast<INT64>(Arg));
            }

            return false;
        }
    };

    template < typename TCharType >
    class TTranslator< TCharType, INT64 > :
        public TTranslatorBase< TCharType, INT64 >
    {
    public:
        typedef TTranslatorBase< TCharType, INT64 >                 Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;

        enum
        {
            MAX_LENGTH = Detail::TMaxLength<LLONG_MAX>::Value
        };

        static bool Transfer(StringType& S, const FormatPattern& Pattern, INT64 Arg, SIZE_T MaxLength = MAX_LENGTH )
        {
            if (Pattern.Flag == FormatPattern::FF_None ||
                Pattern.Flag == FormatPattern::FF_Decimal ||
                Pattern.Flag == FormatPattern::FF_General ||
                Pattern.Flag == FormatPattern::FF_Hex
                )
            {
                const SIZE_T EstimatedLength = Super::EstimateLength(Pattern, MaxLength);
                const SIZE_T Capacity = S.GetCapacity();

                const bool bHex = Pattern.Flag == FormatPattern::FF_Hex;

                if (Capacity >= EstimatedLength)
                {
                    SIZE_T Length = Algorithm::Int64ToString<CharType>(Arg, S.GetUnusedPtr(), bHex ? 16 : 10);

                    Length = Super::AdjustString(S.GetUnusedPtr(), Length, Capacity, Pattern);

                    assert(Length <= Capacity);

                    S.InjectAdd(Length);
                }
                else
                {
                    CharType TempBuf[0xFF];

                    SIZE_T Length = Algorithm::Int64ToString<CharType>(Arg, TempBuf, bHex ? 16 : 10);

                    Length = Super::AdjustString(TempBuf, Length, _countof(TempBuf), Pattern);

                    S.AddStr(TempBuf, TempBuf + Length);
                }

                return true;
            }
            else if (Pattern.Flag == FormatPattern::FF_FixedPoint)
            {
                return TTranslator<TCharType, FLOAT>::Transfer(S, Pattern, static_cast<FLOAT>(Arg));
            }

            return false;
        }
    };

    template < typename TCharType >
    class TTranslator< TCharType, UINT64 > :
        public TTranslatorBase< TCharType, UINT64 >
    {
    public:
        typedef TTranslatorBase< TCharType, UINT64 >                Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;

        enum
        {
            MAX_LENGTH = Detail::TMaxLength<ULLONG_MAX>::Value
        };

        static bool Transfer(StringType& S, const FormatPattern& Pattern, UINT64 Arg, SIZE_T MaxLength = MAX_LENGTH)
        {
            if (Pattern.Flag == FormatPattern::FF_None ||
                Pattern.Flag == FormatPattern::FF_Decimal ||
                Pattern.Flag == FormatPattern::FF_General ||
                Pattern.Flag == FormatPattern::FF_Hex
                )
            {
                const SIZE_T EstimatedLength = Super::EstimateLength(Pattern, MaxLength);
                const SIZE_T Capacity = S.GetCapacity();

                const bool bHex = Pattern.Flag == FormatPattern::FF_Hex;

                if (Capacity >= EstimatedLength)
                {
                    SIZE_T Length = Algorithm::UInt64ToString<CharType>(Arg, S.GetUnusedPtr(), bHex ? 16 : 10);

                    Length = Super::AdjustString(S.GetUnusedPtr(), Length, Capacity, Pattern);

                    assert(Length <= Capacity);

                    S.InjectAdd(Length);
                }
                else
                {
                    CharType TempBuf[0xFF];

                    SIZE_T Length = Algorithm::Int64ToString<CharType>(Arg, TempBuf, bHex ? 16 : 10);

                    Length = Super::AdjustString(TempBuf, Length, _countof(TempBuf), Pattern);

                    S.AddStr(TempBuf, TempBuf + Length);
                }

                return true;
            }
            else if (Pattern.Flag == FormatPattern::FF_FixedPoint)
            {
                return TTranslator<TCharType, FLOAT>::Transfer(S, Pattern, static_cast<FLOAT>(Arg));
            }

            return false;
        }
    };

    template < typename TCharType >
    class TTranslator< TCharType, const TCharType* > :
        public TTranslatorBase< TCharType, const TCharType* >
    {
    public:
        typedef TTranslatorBase< TCharType, const TCharType* >      Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;

        static bool Transfer(StringType& S, const FormatPattern& /*Pattern*/, const TCharType* Str)
        {
            if (Str)
            {
                S.AddStr(Str);
            }

            return true;
        }
    };

#define FL_CONVERT_TRNALSTOR( Type, BaseType, MaxValue ) \
    template < typename TCharType > \
    class TTranslator< TCharType, Type > : \
        public TTranslator< TCharType, BaseType > \
    { \
    public: \
        typedef TTranslator< TCharType, BaseType >  Super; \
        \
        static bool Transfer(typename Super::StringType& S, const typename Super::FormatPattern& Pattern, Type Arg) \
        { \
            return Super::Transfer(S, Pattern, static_cast<BaseType>(Arg), Detail::TMaxLength<MaxValue>::Value); \
        } \
    }

    FL_CONVERT_TRNALSTOR(INT, INT64, INT_MAX);
    FL_CONVERT_TRNALSTOR(UINT, UINT64, UINT_MAX);
    FL_CONVERT_TRNALSTOR(char, INT64, CHAR_MAX);
    FL_CONVERT_TRNALSTOR(unsigned char, UINT64, UCHAR_MAX);
    FL_CONVERT_TRNALSTOR(short, INT64, SHRT_MAX);
    FL_CONVERT_TRNALSTOR(unsigned short, UINT64, USHRT_MAX);
    FL_CONVERT_TRNALSTOR(FLOAT, DOUBLE, 0);

#if FL_COMPILER_MSVC
    FL_CONVERT_TRNALSTOR(DWORD, UINT64, DWORD_MAX);
#elif FL_COMPILER_GCC
    FL_CONVERT_TRNALSTOR(unsigned long, UINT64, ULONG_MAX);
#endif

#undef FL_CONVERT_TRNALSTOR
}

