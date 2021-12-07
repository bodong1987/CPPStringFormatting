// FormatLibrary
// @author bodong
// translator
#pragma once

#if FL_COMPILER_MSVC
#include <intsafe.h>
#else
#include <limits.h>
#endif

/// <summary>
/// The FormatLibrary namespace.
/// </summary>
namespace FormatLibrary
{
    /// <summary>
    /// The Detail namespace.
    /// </summary>
    namespace Detail
    {
        /// <summary>
        /// mpl data provider
        /// </summary>
        template < UINT64 MaxValue >
        struct TMaxLength
        {
            enum
            {
                /// <summary>
                /// The value
                /// </summary>
                Value = TMaxLength<MaxValue / 10>::Value + 1
            };
        };

        /// <summary>
        /// mpl data provider
        /// </summary>
        template <>
        struct TMaxLength<0>
        {
            enum
            {
                /// <summary>
                /// The value
                /// </summary>
                Value = 1
            };
        };
    }

    /// <summary>
    /// base translater
    /// </summary>
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

        /// <summary>
        /// Determines the minimum of the parameters.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <returns>TNumeric.</returns>
        template < typename TNumeric >
        static TNumeric Min( TNumeric x, TNumeric y )
        {
            return x < y?x:y;
        }

        /// <summary>
        /// Determines the maximum of the parameters.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <returns>TNumeric.</returns>
        template < typename TNumeric >
        static TNumeric Max(TNumeric x, TNumeric y)
        {
            return x < y ? y : x;
        }

        /// <summary>
        /// Adjusts the string.
        /// </summary>
        /// <param name="pszStr">The PSZ string.</param>
        /// <param name="length">The length.</param>
        /// <param name="bufferLength">Length of the buffer.</param>
        /// <param name="pattern">The pattern.</param>
        /// <returns>SIZE_T.</returns>
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

        /// <summary>
        /// Estimates the length.
        /// </summary>
        /// <param name="pattern">The pattern.</param>
        /// <param name="maxLength">The maximum length.</param>
        /// <returns>SIZE_T.</returns>
        static SIZE_T EstimateLength(const FormatPattern& pattern, SIZE_T maxLength)
        {
            if (pattern.HasWidth())
            {
                return Max(static_cast<SIZE_T>(pattern.Width), maxLength);
            }

            return maxLength;
        }
    };

    /// <summary>
    /// Class TRawTranslator.
    /// </summary>
    template < typename TCharType >
    class TRawTranslator
    {
    public:
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef Utility::TAutoString<CharType>                      StringType;

        /// <summary>
        /// Raw Translator always success
        /// Transfers the specified string reference.
        /// </summary>
        /// <param name="strRef">The string reference.</param>
        /// <param name="pattern">The pattern.</param>
        /// <param name="format">The format.</param>
        /// <returns>bool.</returns>
        static bool Transfer(StringType& strRef, const FormatPattern& pattern, const CharType* format)
        {
            assert(pattern.Len > 0 && "invalid parameters!!!");

            strRef.AddStr(&format[pattern.Start], &format[pattern.Start] + pattern.Len);

            return true;
        }
    };

    /// <summary>
    /// Class TTranslator.
    /// Implements the <see cref="TTranslatorBase{TCharType, T}" />
    /// </summary>
    /// <seealso cref="TTranslatorBase{TCharType, T}" />
    template < typename TCharType, typename T >
    class TTranslator : public TTranslatorBase< TCharType, T >
    {
    protected:
        typedef TTranslatorBase< TCharType, T > Super;

        /// <summary>
        /// Transfers the specified string reference.
        /// if you get a compile error with this function can't visit
        /// it means that you have transfer an unsupported parameter to format pipeline
        /// you can do them to fix this error:
        ///    1. change your code, convert it to the support type
        ///    2. make a specialization of TTranslator for your type.
        /// </summary>
        /// <param name="strRef">The string reference.</param>
        /// <param name="pattern">The pattern.</param>
        /// <param name="arg">The argument.</param>
        /// <returns>bool.</returns>
        static bool Transfer(typename Super::StringType& strRef, const typename Super::FormatPattern& pattern, const T& arg);
    };

    /// <summary>
    /// bool to string
    /// Implements the <see cref="TTranslatorBase{TCharType, bool}" />
    /// </summary>
    /// <seealso cref="TTranslatorBase{TCharType, bool}" />
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

        /// <summary>
        /// Transfers the specified string reference.
        /// </summary>
        /// <param name="strRef">The string reference.</param>
        /// <param name="pattern">The pattern.</param>
        /// <param name="arg">The argument.</param>
        /// <returns>bool.</returns>
        static bool Transfer(StringType& strRef, const FormatPattern& pattern, bool arg)
        {
            static const TCharType TrueStr[] = { 'T', 'r', 'u', 'e' };
            static const TCharType FalseStr[] = { 'F', 'a', 'l', 's', 'e' };

            const TCharType* Selection = arg ? TrueStr : FalseStr;
            const SIZE_T length = arg ? _countof(TrueStr) : _countof(FalseStr);

            strRef.AddStr(Selection, Selection + length);

            return true;
        }
    };

    enum
    {
        /// <summary>
        /// The default float precision
        /// </summary>
        DEFAULT_FLOAT_PRECISION = 2
    };

    /// <summary>
    /// double to string
    /// Implements the <see cref="TTranslatorBase{TCharType, DOUBLE}" />
    /// </summary>
    /// <seealso cref="TTranslatorBase{TCharType, DOUBLE}" />
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

        /// <summary>
        /// Transfers the specified string reference.
        /// </summary>
        /// <param name="strRef">The string reference.</param>
        /// <param name="pattern">The pattern.</param>
        /// <param name="arg">The argument.</param>
        /// <param name="unusedParam">The unused parameter.</param>
        /// <returns>bool.</returns>
        static bool Transfer(StringType& strRef, const FormatPattern& pattern, DOUBLE arg, SIZE_T unusedParam = 0)
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
                        pattern.HasPrecision() ? pattern.Precision : DEFAULT_FLOAT_PRECISION
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
                        pattern.HasPrecision() ? pattern.Precision : DEFAULT_FLOAT_PRECISION
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

    /// <summary>
    /// Int64 to string
    /// Implements the <see cref="TTranslatorBase{TCharType, INT64}" />
    /// </summary>
    /// <seealso cref="TTranslatorBase{TCharType, INT64}" />
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
            /// <summary>
            /// The maximum length
            /// </summary>
            MAX_LENGTH = Detail::TMaxLength<LLONG_MAX>::Value
        };

        /// <summary>
        /// Transfers the specified string reference.
        /// </summary>
        /// <param name="strRef">The string reference.</param>
        /// <param name="pattern">The pattern.</param>
        /// <param name="arg">The argument.</param>
        /// <param name="maxLength">The maximum length.</param>
        /// <returns>bool.</returns>
        static bool Transfer(StringType& strRef, const FormatPattern& pattern, INT64 arg, SIZE_T maxLength = MAX_LENGTH)
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

    /// <summary>
    /// uint64 to string
    /// Implements the <see cref="TTranslatorBase{TCharType, UINT64}" />
    /// </summary>
    /// <seealso cref="TTranslatorBase{TCharType, UINT64}" />
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
            /// <summary>
            /// The maximum length
            /// </summary>
            MAX_LENGTH = Detail::TMaxLength<ULLONG_MAX>::Value
        };

        /// <summary>
        /// Transfers the specified string reference.
        /// </summary>
        /// <param name="strRef">The string reference.</param>
        /// <param name="pattern">The pattern.</param>
        /// <param name="arg">The argument.</param>
        /// <param name="maxLength">The maximum length.</param>
        /// <returns>bool.</returns>
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

    /// <summary>
    /// const char* to string
    /// Implements the <see cref="TTranslatorBase{TCharType, const TCharType*}" />
    /// </summary>
    /// <seealso cref="TTranslatorBase{TCharType, const TCharType*}" />
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

        /// <summary>
        /// Transfers the specified string reference.
        /// </summary>
        /// <param name="strRef">The string reference.</param>
        /// <param name="">The .</param>
        /// <param name="str">The string.</param>
        /// <returns>bool.</returns>
        static bool Transfer(StringType& strRef, const FormatPattern& /*pattern*/, const TCharType* str)
        {
            if (str)
            {
                strRef.AddStr(str);
            }

            return true;
        }
    };

    // convert small numeric type to big numeric type 
    // and convert them to string
#define FL_CONVERT_TRANSLATOR( Type, BaseType, MaxValue ) \
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

    FL_CONVERT_TRANSLATOR(INT, INT64, INT_MAX);
    FL_CONVERT_TRANSLATOR(UINT, UINT64, UINT_MAX);
    FL_CONVERT_TRANSLATOR(char, INT64, CHAR_MAX);
    FL_CONVERT_TRANSLATOR(unsigned char, UINT64, UCHAR_MAX);
    FL_CONVERT_TRANSLATOR(short, INT64, SHRT_MAX);
    FL_CONVERT_TRANSLATOR(unsigned short, UINT64, USHRT_MAX);    
    FL_CONVERT_TRANSLATOR(FLOAT, DOUBLE, 0);

#if FL_COMPILER_MSVC
    FL_CONVERT_TRANSLATOR(DWORD, UINT64, DWORD_MAX);
#elif FL_COMPILER_GCC
    FL_CONVERT_TRANSLATOR(unsigned long, UINT64, ULONG_MAX);
#endif

#undef FL_CONVERT_TRNALSTOR
}

