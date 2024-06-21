#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Details/Algorithm.hpp>
#include <Format/Details/Pattern.hpp>
#include <Format/Common/AutoString.hpp>

namespace FormatLibrary
{
    namespace Details
    {        
        template <uint64_t MaxValue>
        struct TMaxLength
        {
            enum
            {                
                Value = TMaxLength<MaxValue / 10>::Value + 1
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

        template < typename TCharType, typename T >
        class TTranslatorBase
        {
        public:
            typedef TCharType                                           CharType;
            typedef TFormatPattern<TCharType>                           FormatPattern;
            typedef typename FormatPattern::ByteType                    ByteType;
            typedef typename FormatPattern::SizeType                    SizeType;
            typedef TAutoString<CharType>                               StringType;
            typedef TCharTraits<CharType>                               CharTraits;          
            
        protected:
            static inline void AppendString(StringType& strRef, const FormatPattern& pattern, const CharType* start, const SizeType length, CharType fillChar)
            {
                strRef.AddAlignStr(start, start + length, pattern.HasWidth()? pattern.Width : (int)length, pattern.Align != EAlignFlag::Left, fillChar);
            }
        };

        template < typename TCharType >
        class TRawTranslator
        {
        public:
            typedef TCharType                                           CharType;
            typedef TFormatPattern<TCharType>                           FormatPattern;
            typedef TAutoString<CharType>                               StringType;

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

        template < typename TCharType, typename T >
        class TTranslator : public TTranslatorBase< TCharType, T >
        {
        public:
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
            
            FL_STATIC_ASSERT(false, "You may be trying to format an unsupported type");

#if FL_COMPILER_WITH_CXX11
            static bool Transfer(typename Super::StringType& strRef, const typename Super::FormatPattern& pattern, const T& arg) = delete;
#else
        private:
            static bool Transfer(typename Super::StringType& strRef, const typename Super::FormatPattern& pattern, const T& arg);
#endif
        };

        // convert bool to string
        template < typename TCharType >
        class TTranslator< TCharType, bool > :
            public TTranslatorBase< TCharType, bool >
        {
        public:
            typedef TTranslatorBase< TCharType, bool >                  Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, bool arg)
            {
                static const TCharType TrueStr[] = { 'T', 'r', 'u', 'e' };
                static const TCharType FalseStr[] = { 'F', 'a', 'l', 's', 'e' };

                const TCharType* Selection = arg ? TrueStr : FalseStr;
                const SizeType length = arg ? _countof(TrueStr) : _countof(FalseStr);

                Super::AppendString(strRef, pattern, Selection, length, CharTraits::GetSpace());

                return true;
            }
        };

        // convert TCharType to string
        template < typename TCharType >
        class TTranslator< TCharType, TCharType > :
            public TTranslatorBase< TCharType, TCharType >
        {
        public:
            typedef TTranslatorBase< TCharType, TCharType >             Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, TCharType arg)
            {
                TCharType Buffer[2] = {arg, (TCharType)0};

                Super::AppendString(strRef, pattern, Buffer, 1, CharTraits::GetSpace());

                return true;
            }
        };

        #ifndef FL_DEFAULT_FLOAT_PRECISION
        #define FL_DEFAULT_FLOAT_PRECISION 2
        #endif

        enum
        {
            DEFAULT_FLOAT_PRECISION = FL_DEFAULT_FLOAT_PRECISION
        };

        // convert double to string
        template < typename TCharType >
        class TTranslator< TCharType, double > :
            public TTranslatorBase< TCharType, double >
        {
        public:
            typedef TTranslatorBase< TCharType, double >                Super;            
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, double arg, SizeType unusedParam = 0)
            {
                FL_UNREFERENCED_PARAMETER(unusedParam);

                if (pattern.Flag == EFormatFlag::None ||
                    pattern.Flag == EFormatFlag::FixedPoint ||
                    pattern.Flag == EFormatFlag::General
                    )
                {
                    CharType TempBuf[64];

                    SizeType length = DoubleToString<CharType>(
                        arg,
                        TempBuf,
                        _countof(TempBuf),
                        pattern.HasPrecision() ? pattern.Precision : DEFAULT_FLOAT_PRECISION
                    );

                    Super::AppendString(strRef, pattern, TempBuf, length, CharTraits::GetZero());
                }
                else if (pattern.Flag == EFormatFlag::Decimal)
                {
                    return TTranslator<TCharType, int64_t>::Transfer(strRef, pattern, static_cast<int64_t>(arg));
                }

                return false;
            }
        };

        /// <summary>
        /// Int64 to string
        /// Implements the <see cref="TTranslatorBase{TCharType, int64_t}" />
        /// </summary>
        /// <seealso cref="TTranslatorBase{TCharType, int64_t}" />
        template < typename TCharType >
        class TTranslator< TCharType, int64_t > :
            public TTranslatorBase< TCharType, int64_t >
        {
        public:
            typedef TTranslatorBase< TCharType, int64_t >               Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

            enum
            {
                MAX_LENGTH = TMaxLength<LLONG_MAX>::Value
            };

            /// <summary>
            /// Transfers the specified string reference.
            /// </summary>
            /// <param name="strRef">The string reference.</param>
            /// <param name="pattern">The pattern.</param>
            /// <param name="arg">The argument.</param>
            /// <param name="maxLength">The maximum length.</param>
            /// <returns>bool.</returns>
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, int64_t arg, SizeType maxLength = MAX_LENGTH)
            {
                if (pattern.Flag == EFormatFlag::None ||
                    pattern.Flag == EFormatFlag::Decimal ||
                    pattern.Flag == EFormatFlag::General ||
                    pattern.Flag == EFormatFlag::Hex
                    )
                {
                    const bool bHex = pattern.Flag == EFormatFlag::Hex;

                    CharType TempBuf[32];

                    SizeType length = Int64ToString<CharType>(arg, TempBuf, bHex ? 16 : 10);

                    Super::AppendString(strRef, pattern, TempBuf, length, CharTraits::GetZero());

                    return true;
                }
                else if (pattern.Flag == EFormatFlag::FixedPoint)
                {
                    return TTranslator<TCharType, float>::Transfer(strRef, pattern, static_cast<float>(arg));
                }

                return false;
            }
        };

        /// <summary>
        /// uint64 to string
        /// Implements the <see cref="TTranslatorBase{TCharType, uint64_t}" />
        /// </summary>
        /// <seealso cref="TTranslatorBase{TCharType, uint64_t}" />
        template < typename TCharType >
        class TTranslator< TCharType, uint64_t > :
            public TTranslatorBase< TCharType, uint64_t >
        {
        public:
            typedef TTranslatorBase< TCharType, uint64_t >              Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

            enum
            {
                MAX_LENGTH = TMaxLength<ULLONG_MAX>::Value
            };

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, uint64_t arg, SizeType maxLength = MAX_LENGTH)
            {
                if (pattern.Flag == EFormatFlag::None ||
                    pattern.Flag == EFormatFlag::Decimal ||
                    pattern.Flag == EFormatFlag::General ||
                    pattern.Flag == EFormatFlag::Hex
                    )
                {
                    const bool bHex = pattern.Flag == EFormatFlag::Hex;

                    CharType TempBuf[32];

                    SizeType length = Int64ToString<CharType>(arg, TempBuf, bHex ? 16 : 10);

                    Super::AppendString(strRef, pattern, TempBuf, length, CharTraits::GetZero());

                    return true;
                }
                else if (pattern.Flag == EFormatFlag::FixedPoint)
                {
                    return TTranslator<TCharType, float>::Transfer(strRef, pattern, static_cast<float>(arg));
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
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

            /// <summary>
            /// Transfers the specified string reference.
            /// </summary>
            /// <param name="strRef">The string reference.</param>
            /// <param name="">The .</param>
            /// <param name="str">The string.</param>
            /// <returns>bool.</returns>
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, const TCharType* str)
            {
                if (str)
                {
                    Super::AppendString(strRef, pattern, str, CharTraits::length(str), CharTraits::GetSpace());
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
            return Super::Transfer(strRef, pattern, static_cast<BaseType>(arg), Details::TMaxLength<MaxValue>::Value); \
        } \
    }

        FL_CONVERT_TRANSLATOR(int32_t, int64_t, INT_MAX);
        FL_CONVERT_TRANSLATOR(uint32_t, uint64_t, UINT_MAX);
        FL_CONVERT_TRANSLATOR(uint8_t, uint64_t, UCHAR_MAX);
        FL_CONVERT_TRANSLATOR(int16_t, int64_t, SHRT_MAX);
        FL_CONVERT_TRANSLATOR(uint16_t, uint64_t, USHRT_MAX);
        FL_CONVERT_TRANSLATOR(float, double, 0);
        FL_CONVERT_TRANSLATOR(long, int64_t, INT_MAX);
        FL_CONVERT_TRANSLATOR(unsigned long, uint64_t, ULONG_MAX);

#undef FL_CONVERT_TRANSLATOR

    }
}
