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
            SIZE_T length,
            const SIZE_T bufferLength,
            const FormatPattern& pattern
            )
        {
            SIZE_T UsedLength = length;

            if (pattern.HasWidth() && pattern.Width > length)
            {
                const SIZE_T PadCount = Min(bufferLength - length, pattern.Width - length);

                assert(PadCount < 0xFF);

                if (pattern.Align == FormatPattern::AF_Left)
                {
                    // Fill at the end
                    CharTraits::Fill(pszStr + length, ' ', PadCount);

                    pszStr[length + PadCount] = 0;
                }
                else
                {
                    // fill at the start
                    CharTraits::copy(pszStr + PadCount, pszStr, length);
                    CharTraits::Fill(pszStr, ' ', PadCount);
                    pszStr[length + PadCount] = 0;
                }

                UsedLength = length + PadCount;
            }

            return UsedLength;
        }

        static SIZE_T EstimateLength(const FormatPattern& pattern, SIZE_T maxLength)
        {
            if (pattern.HasWidth())
            {
                return Max(static_cast<SIZE_T>(pattern.Width), maxLength);
            }

            return maxLength;
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
        static bool Transfer(StringType& strRef, const FormatPattern& pattern, const CharType* format)
        {
            assert(pattern.Len > 0 && "invalid parameters!!!");

            strRef.AddStr(&format[pattern.Start], &format[pattern.Start] + pattern.Len);

            return true;
        }
    };

    template < typename TCharType, typename T >
    class TTranslator : public TTranslatorBase< TCharType, T >
    {
    protected:
        typedef TTranslatorBase< TCharType, T > Super;

        // if you get a compile error with this function can't visit
        // it means that you have transfer an unsupported parameter to format pipeline
        // you can do them to fix this error:
        //    1. change your code, convert it to the support type
        //    2. make a specialization of TTranslator for your type.
        static bool Transfer(typename Super::StringType& strRef, const typename Super::FormatPattern& pattern, const T& arg);
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

        static bool Transfer(StringType& strRef, const FormatPattern& pattern, bool arg)
        {
            TCharType TrueStr[] = { 'T', 'r', 'u', 'e' };
            TCharType FalseStr[] = { 'F', 'a', 'l', 's', 'e' };

            const TCharType* Selection = arg ? TrueStr : FalseStr;
            const SIZE_T length = arg ? _countof(TrueStr) : _countof(FalseStr);

            strRef.AddStr(Selection, Selection + length);

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

        static bool Transfer(StringType& strRef, const FormatPattern& pattern, DOUBLE arg, SIZE_T unusedParam = 0 )
        {
            FL_UNREFERENCED_PARAMETER(unusedParam);

            if (pattern.Flag == FormatPattern::FF_None ||
                pattern.Flag == FormatPattern::FF_FixedPoint ||
                pattern.Flag == FormatPattern::FF_General
                )
            {
                // assume Double value will less than 32
                const SIZE_T EstimatedLength = Super::EstimateLength(pattern, 32);
                const SIZE_T Capacity = strRef.GetCapacity();

                if (Capacity >= EstimatedLength)
                {
                    SIZE_T length = Algorithm::DoubleToString<CharType>(
                        arg,
                        strRef.GetUnusedPtr(),
                        Capacity,
                        pattern.HasPrecision() ? pattern.Precision : DEFAULT_FLOAT_PERCISION
                        );

                    length = Super::AdjustString(strRef.GetUnusedPtr(), length, Capacity, pattern);

                    assert(length <= Capacity);

                    strRef.InjectAdd(length);
                }
                else
                {
                    CharType TempBuf[0xFF];

                    SIZE_T length = Algorithm::DoubleToString<CharType>(
                        arg,
                        TempBuf,
                        _countof(TempBuf),
                        pattern.HasPrecision() ? pattern.Precision : DEFAULT_FLOAT_PERCISION
                        );

                    length = Super::AdjustString(TempBuf, length, _countof(TempBuf), pattern);

                    strRef.AddStr(TempBuf, TempBuf + length);
                }

                return true;
            }
            else if (pattern.Flag == FormatPattern::FF_Decimal)
            {
                return TTranslator<TCharType, INT64>::Transfer(strRef, pattern, static_cast<INT64>(arg));
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

        static bool Transfer(StringType& strRef, const FormatPattern& pattern, INT64 arg, SIZE_T maxLength = MAX_LENGTH )
        {
            if (pattern.Flag == FormatPattern::FF_None ||
                pattern.Flag == FormatPattern::FF_Decimal ||
                pattern.Flag == FormatPattern::FF_General ||
                pattern.Flag == FormatPattern::FF_Hex
                )
            {
                const SIZE_T EstimatedLength = Super::EstimateLength(pattern, maxLength);
                const SIZE_T Capacity = strRef.GetCapacity();

                const bool bHex = pattern.Flag == FormatPattern::FF_Hex;

                if (Capacity >= EstimatedLength)
                {
                    SIZE_T length = Algorithm::Int64ToString<CharType>(arg, strRef.GetUnusedPtr(), bHex ? 16 : 10);

                    length = Super::AdjustString(strRef.GetUnusedPtr(), length, Capacity, pattern);

                    assert(length <= Capacity);

                    strRef.InjectAdd(length);
                }
                else
                {
                    CharType TempBuf[0xFF];

                    SIZE_T length = Algorithm::Int64ToString<CharType>(arg, TempBuf, bHex ? 16 : 10);

                    length = Super::AdjustString(TempBuf, length, _countof(TempBuf), pattern);

                    strRef.AddStr(TempBuf, TempBuf + length);
                }

                return true;
            }
            else if (pattern.Flag == FormatPattern::FF_FixedPoint)
            {
                return TTranslator<TCharType, FLOAT>::Transfer(strRef, pattern, static_cast<FLOAT>(arg));
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

        static bool Transfer(StringType& strRef, const FormatPattern& pattern, UINT64 arg, SIZE_T maxLength = MAX_LENGTH)
        {
            if (pattern.Flag == FormatPattern::FF_None ||
                pattern.Flag == FormatPattern::FF_Decimal ||
                pattern.Flag == FormatPattern::FF_General ||
                pattern.Flag == FormatPattern::FF_Hex
                )
            {
                const SIZE_T EstimatedLength = Super::EstimateLength(pattern, maxLength);
                const SIZE_T Capacity = strRef.GetCapacity();

                const bool bHex = pattern.Flag == FormatPattern::FF_Hex;

                if (Capacity >= EstimatedLength)
                {
                    SIZE_T length = Algorithm::UInt64ToString<CharType>(arg, strRef.GetUnusedPtr(), bHex ? 16 : 10);

                    length = Super::AdjustString(strRef.GetUnusedPtr(), length, Capacity, pattern);

                    assert(length <= Capacity);

                    strRef.InjectAdd(length);
                }
                else
                {
                    CharType TempBuf[0xFF];

                    SIZE_T length = Algorithm::Int64ToString<CharType>(arg, TempBuf, bHex ? 16 : 10);

                    length = Super::AdjustString(TempBuf, length, _countof(TempBuf), pattern);

                    strRef.AddStr(TempBuf, TempBuf + length);
                }

                return true;
            }
            else if (pattern.Flag == FormatPattern::FF_FixedPoint)
            {
                return TTranslator<TCharType, FLOAT>::Transfer(strRef, pattern, static_cast<FLOAT>(arg));
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

        static bool Transfer(StringType& strRef, const FormatPattern& /*pattern*/, const TCharType* str)
        {
            if (str)
            {
                strRef.AddStr(str);
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
        static bool Transfer(typename Super::StringType& strRef, const typename Super::FormatPattern& pattern, Type arg) \
        { \
            return Super::Transfer(strRef, pattern, static_cast<BaseType>(arg), Detail::TMaxLength<MaxValue>::Value); \
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

