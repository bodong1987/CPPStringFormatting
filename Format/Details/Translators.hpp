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
// ReSharper disable CppRedundantElseKeywordInsideCompoundStatement
// ReSharper disable CppRedundantElseKeyword
#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Details/StringConvertAlgorithm.hpp>
#include <Format/Details/Pattern.hpp>
#include <Format/Common/AutoString.hpp>
#include <Format/Common/Mpl.hpp>

// ReSharper disable once CppUnusedIncludeDirective
#include <Format/Common/Macros.hpp>

// ReSharper disable once CppEnforceNestedNamespacesStyle
namespace Formatting // NOLINT(*-concat-nested-namespaces)
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

        /// <summary>
        /// Class TTranslatorBase.
        /// provide base interfaces
        /// </summary>
        template < typename TCharType, typename T >
        class TTranslatorBase
        {
        public:
            typedef TCharType                                           CharType;
            typedef T                                                   ParameterType;
            typedef TFormatPattern<TCharType>                           FormatPattern;
            typedef typename FormatPattern::ByteType                    ByteType;
            typedef typename FormatPattern::SizeType                    SizeType;
            typedef TAutoString<CharType>                               StringType;
            typedef TCharTraits<CharType>                               CharTraits;

        protected:
            static void AppendString(
                StringType& strRef,
                const FormatPattern& pattern,
                const CharType* start,
                const SizeType length,
                CharType fillChar = CharTraits::GetSpace()
                )
            {
                strRef.AddAlignStr(
                    start,
                    length,
                    pattern.HasWidth() ? pattern.Width : static_cast<int>(length),
                    pattern.Align != EAlignFlag::Left,
                    fillChar
                    );
            }

            static void AppendString(
                StringType& strRef,
                const FormatPattern& pattern,
                const CharType* start,
                const SizeType length,
                const int alignSize,
                bool paddingLeft,
                CharType fillChar = CharTraits::GetSpace()
                )
            {
                FL_UNREFERENCED_PARAMETER(pattern);

                strRef.AddAlignStr(
                    start,
                    length,
                    alignSize,
                    paddingLeft,
                    fillChar
                    );
            }
        };

        /// <summary>
        /// Class TRawTranslator.
        /// raw translator, transfer raw string to buffer
        /// </summary>
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

                strRef.AddStr(&format[pattern.Start], pattern.Len);

                return true;
            }
        };

        /// <summary>
        /// Class TTranslator.
        /// invalid translator placeholder
        /// Implements the <see cref="TTranslatorBase{TCharType, T}" />
        /// </summary>
        /// <seealso cref="TTranslatorBase{TCharType, T}" />
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

#if FL_COMPILER_IS_GREATER_THAN_CXX11
            static bool Transfer(
                typename Super::StringType& strRef,
                const typename Super::FormatPattern& pattern,
                const T& arg
                ) = delete;
#else
        private:
            static bool Transfer(
                typename Super::StringType& strRef,
                const typename Super::FormatPattern& pattern,
                const T& arg
                );
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
                const SizeType length = arg ? FL_ARRAY_COUNTOF(TrueStr) : FL_ARRAY_COUNTOF(FalseStr);

                Super::AppendString(strRef, pattern, Selection, length);

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
                TCharType Buffer[2] = { arg, TCharType(0) };

                Super::AppendString(strRef, pattern, Buffer, 1);

                return true;
            }
        };

        // convert double to string
        template < typename TCharType, typename TRealType >
        class TDoubleTranslatorImpl : public TTranslatorBase< TCharType, double >
        {
        public:
            typedef TTranslatorBase< TCharType, double >                Super;
            typedef TRealType                                           RealType;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

        private:
#ifndef FL_DOUBLE_TRANSLATOR_DEFAULT_PRECISION
#define FL_DOUBLE_TRANSLATOR_DEFAULT_PRECISION 2
#endif

#ifndef FL_DOUBLE_TRANSLATOR_DEFAULT_FIXED_POINT_PRECISION
#define FL_DOUBLE_TRANSLATOR_DEFAULT_FIXED_POINT_PRECISION 2
#endif

#ifndef FL_DOUBLE_TRANSLATOR_DEFAULT_EXPONENT_PRECISION
#define FL_DOUBLE_TRANSLATOR_DEFAULT_EXPONENT_PRECISION 6
#endif

#ifndef FL_DOUBLE_TRANSLATOR_DEFAULT_MIN_EXPONENT_LENGTH
#define FL_DOUBLE_TRANSLATOR_DEFAULT_MIN_EXPONENT_LENGTH 5
#endif

            enum
            {
                DefaultPrecision = FL_DOUBLE_TRANSLATOR_DEFAULT_PRECISION,
                DefaultFixedPointPrecision = FL_DOUBLE_TRANSLATOR_DEFAULT_FIXED_POINT_PRECISION,
                DefaultExponentPrecision = FL_DOUBLE_TRANSLATOR_DEFAULT_EXPONENT_PRECISION,
                DefaultMinExponentLength = FL_DOUBLE_TRANSLATOR_DEFAULT_MIN_EXPONENT_LENGTH
            };

            static bool TransferGeneral(StringType& strRef, const FormatPattern& pattern, const double arg)
            {
                CharType TempBuf[64];

                const CharType* const Result = DoubleToString<CharType>(
                    arg,
                    TempBuf,
                    FL_ARRAY_COUNTOF(TempBuf),
                    pattern.HasPrecision() ?
                        pattern.Precision :
                        (
                            pattern.Flag == EFormatFlag::FixedPoint ?
                            DefaultFixedPointPrecision :
                            DefaultPrecision
                        ) // NOLINT
                );

                const SizeType length = CalculateConvertedStringLength(Result, TempBuf);

                Super::AppendString(strRef, pattern, Result, length);

                return true;
            }

            static bool TransferExponent(StringType& strRef, const FormatPattern& pattern, const double arg)
            {
                const CharType eFlagChar = pattern.IsUpper ? 'E' : 'e';

                // add %.
                CharType FmtBuf[16];
                FmtBuf[0] = '%';
                FmtBuf[1] = '.';
                // calculate precision number, add append to FmtBuffer
                CharType PrecisionBuf[16];
                const CharType* const PrecisionText =
                    IntegerToString<TCharType, int, 10>(
                        pattern.HasPrecision() ? pattern.Precision : DefaultExponentPrecision, PrecisionBuf,
                        FL_ARRAY_COUNTOF(PrecisionBuf),
                        pattern.IsUpper
                        );

                const SizeType length = CalculateConvertedStringLength(PrecisionText, PrecisionBuf);

                TCharTraits<CharType>::copy(FmtBuf + 2, PrecisionText, length);

                // add e flag
                FmtBuf[length + 2] = eFlagChar;
                FmtBuf[length + 3] = CharTraits::GetEndFlag();

                // format by sprintf
                CharType TempBuf[64];
                size_t bufLength = CharTraits::StringPrintf(TempBuf, FmtBuf, arg);

                // process alignment.
                const CharType* plusPos = CharTraits::rFind(TempBuf, eFlagChar);
                if (plusPos != nullptr)
                {
                    const CharType* endPos = TempBuf + bufLength;
                    const int exponentLength = static_cast<int>(endPos - plusPos);

                    if (exponentLength < DefaultMinExponentLength)
                    {
                        const int padLength = DefaultMinExponentLength - exponentLength;

                        memmove(const_cast<CharType*>(plusPos + padLength + 2),
                            plusPos + 2,
                            exponentLength * sizeof(CharType)
                            );

                        CharTraits::Fill(const_cast<CharType*>(plusPos + 2), CharTraits::GetZero(), padLength);

                        bufLength += padLength;
                    }
                }

                Super::AppendString(strRef, pattern, TempBuf, bufLength);

                return true;
            }

        public:
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, const double arg)
            {
                switch (pattern.Flag)
                {
                case EFormatFlag::General:
                case EFormatFlag::FixedPoint:
                case EFormatFlag::None:
                    return TransferGeneral(strRef, pattern, arg);
                case EFormatFlag::Exponent:
                    return TransferExponent(strRef, pattern, arg);
                case EFormatFlag::Decimal:
                    return TTranslator<TCharType, int64_t>::Transfer(strRef, pattern, static_cast<int64_t>(arg));
                case EFormatFlag::Binary:
                    assert(false && "double to binary string is not supported.");
                default:
                    break;
                }

                return false;
            }
        };

        // convert float to string
        template < typename TCharType >
        class TTranslator< TCharType, float > :
            public TTranslatorBase< TCharType, float >
        {
        public:
            typedef TTranslatorBase< TCharType, float >                 Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, float arg)
            {
                return TDoubleTranslatorImpl<TCharType, float>::Transfer(strRef, pattern, arg);
            }
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

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, double arg)
            {
                return TDoubleTranslatorImpl<TCharType, double>::Transfer(strRef, pattern, arg);
            }
        };

        /// <summary>
        /// Class TIntegerTranslatorImpl.
        /// </summary>
        template < typename TCharType, typename TIntegerType >
        class TIntegerTranslatorImpl : public TTranslatorBase<TCharType, TIntegerType>
        {
        public:
            typedef TTranslatorBase< TCharType, TIntegerType>           Super;
            typedef typename Super::ParameterType                       ParameterType;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

        private:
            static bool TransferGeneral(StringType& strRef, const FormatPattern& pattern, ParameterType arg)
            {
                const bool bHex = pattern.Flag == EFormatFlag::Hex;

                CharType TempBuf[32];

                const CharType* const Result = bHex ?
                    IntegerToString<CharType, ParameterType, 16>(
                        arg,
                        TempBuf,
                        FL_ARRAY_COUNTOF(TempBuf),
                        pattern.IsUpper
                        ) :
                    IntegerToString<CharType, ParameterType, 10>(
                        arg,
                        TempBuf,
                        FL_ARRAY_COUNTOF(TempBuf),
                        pattern.IsUpper
                        );

                const SizeType length = CalculateConvertedStringLength(Result, TempBuf);

                if (pattern.HasPrecision() && pattern.Precision > length)
                {
                    Super::AppendString(
                        strRef,
                        pattern,
                        Result,
                        length,
                        pattern.Precision,
                        true,
                        CharTraits::GetZero()
                        );
                }
                else
                {
                    Super::AppendString(strRef, pattern, Result, length);
                }

                return true;
            }

            static bool TransferBinary(StringType& strRef, const FormatPattern& pattern, ParameterType arg)
            {
                constexpr int length = sizeof(ParameterType) * 8;
                CharType TempBuf[length + 1];

                const int usedLength =
                    IntegerToBinaryString<TCharType, ParameterType>(static_cast<ParameterType>(arg), TempBuf);

                const TCharType* const Result = TempBuf + (length - usedLength);

                if (pattern.HasPrecision() && pattern.Precision > usedLength)
                {
                    Super::AppendString(
                        strRef,
                        pattern,
                        Result,
                        usedLength,
                        pattern.Precision,
                        true,
                        CharTraits::GetZero()
                        );
                }
                else
                {
                    Super::AppendString(strRef, pattern, Result, usedLength);
                }

                return true;
            }

        public:
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, ParameterType arg)
            {
                switch (pattern.Flag)
                {
                case EFormatFlag::General:
                case EFormatFlag::Decimal:
                case EFormatFlag::Hex:
                case EFormatFlag::None:
                    return TransferGeneral(strRef, pattern, arg);
                case EFormatFlag::Exponent:
                    return TTranslator<TCharType, double>::Transfer(strRef, pattern, static_cast<double>(arg));
                case EFormatFlag::FixedPoint:
                    return TTranslator<TCharType, float>::Transfer(strRef, pattern, static_cast<float>(arg));
                case EFormatFlag::Binary:
                    return TransferBinary(strRef, pattern, arg);
                default:
                    break;
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

            /// <summary>
            /// Transfers the specified string reference.
            /// </summary>
            /// <param name="strRef">The string reference.</param>
            /// <param name="pattern">The pattern.</param>
            /// <param name="arg">The argument.</param>
            /// <returns>bool.</returns>
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, int64_t arg)
            {
                return TIntegerTranslatorImpl<TCharType, int64_t>::Transfer(strRef, pattern, arg);
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

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, uint64_t arg)
            {
                return TIntegerTranslatorImpl<TCharType, uint64_t>::Transfer(strRef, pattern, arg);
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
            /// <param name="pattern">pattern</param>
            /// <param name="str">The string.</param>
            /// <returns>bool.</returns>
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, const TCharType* str)
            {
                if (str)
                {
                    Super::AppendString(strRef, pattern, str, CharTraits::length(str));
                }

                return true;
            }
        };

        /// <summary>
        /// Class TPointerTranslatorImpl.
        /// process common pointer format
        /// Implements the <see cref="TTranslatorBase{TCharType, T}" />
        /// </summary>
        /// <seealso cref="TTranslatorBase{TCharType, T}" />
        template < typename TCharType, typename T>
        class TPointerTranslatorImpl : public TTranslatorBase<TCharType, T> // NOLINT
        {
        public:
            typedef TTranslatorBase< TCharType, T>                      Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, T ptr)
            {
                FL_STATIC_ASSERT(Mpl::IsPtr<T>::Value, "must be pointer type");

                assert(pattern.Len > 0 && "invalid parameters!!!");

                const bool bHex = pattern.Flag == EFormatFlag::Hex || pattern.Flag == EFormatFlag::None;
                const size_t arg = reinterpret_cast<size_t>(ptr); // NOLINT(*-use-auto)

                CharType TempBuf[32];
                const CharType* const Result = bHex ?
                        IntegerToString<CharType, size_t, 16>(
                            arg,
                            TempBuf,
                            FL_ARRAY_COUNTOF(TempBuf),
                            pattern.IsUpper
                            ) :
                        IntegerToString<CharType, size_t, 10>(
                            arg,
                            TempBuf,
                            FL_ARRAY_COUNTOF(TempBuf),
                            pattern.IsUpper
                            );

                const SizeType length = CalculateConvertedStringLength(Result, TempBuf);

                if (pattern.HasPrecision() && pattern.Precision > length)
                {
                    Super::AppendString(
                        strRef,
                        pattern,
                        Result,
                        length,
                        pattern.Precision,
                        true,
                        CharTraits::GetZero()
                        );
                }
                else
                {
                    constexpr size_t defaultAlignedLength = sizeof(void*)*2;

                    Super::AppendString(
                        strRef,
                        pattern,
                        Result,
                        length,
                        defaultAlignedLength,
                        true,
                        CharTraits::GetZero()
                        );
                }

                return true;
            }
        };

        // for const void*
        template <typename TCharType>
        class TTranslator<TCharType, const void*> : public TTranslatorBase<TCharType, const void*>
        {
            typedef TTranslatorBase< TCharType, const void*>            Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;
        public:
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, const void* ptr)
            {
                return TPointerTranslatorImpl<TCharType, const void*>::Transfer(strRef, pattern, ptr);
            }
        };

        // for void*
        template <typename TCharType>
        class TTranslator<TCharType, void*> : public TTranslatorBase<TCharType, void*>
        {
            typedef TTranslatorBase< TCharType, void*>                  Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;
        public:
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, void* ptr)
            {
                return TPointerTranslatorImpl<TCharType, void*>::Transfer(strRef, pattern, ptr);
            }
        };

        // for T*
        template <typename TCharType, typename T>
        class TTranslator<TCharType, T*> : public TTranslatorBase<TCharType, T*>
        {
            typedef TTranslatorBase< TCharType, T*>                     Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;
        public:
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, T* ptr)
            {
                return TPointerTranslatorImpl<TCharType, T*>::Transfer(strRef, pattern, ptr);
            }
        };

        // for const T*
        template <typename TCharType, typename T>
        class TTranslator<TCharType, const T*> : public TTranslatorBase<TCharType, const T*>
        {
            typedef TTranslatorBase< TCharType, const T*>               Super;
            typedef typename Super::CharType                            CharType;
            typedef typename Super::FormatPattern                       FormatPattern;
            typedef typename Super::ByteType                            ByteType;
            typedef typename Super::SizeType                            SizeType;
            typedef typename Super::StringType                          StringType;
            typedef typename Super::CharTraits                          CharTraits;
        public:
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, const T* ptr)
            {
                return TPointerTranslatorImpl<TCharType, const T*>::Transfer(strRef, pattern, ptr);
            }
        };

        // convert small numeric type to big numeric type
        // and convert them to string
#define FL_CONVERT_TRANSLATOR(Type, BaseType, baseTranslatorType, ImplType) \
    template < typename TCharType > \
    class TTranslator< TCharType, Type > : \
        public baseTranslatorType< TCharType, BaseType > /*NOLINT(bugprone-macro-parentheses)*/ \
    { \
    public: \
        typedef TTranslator< TCharType, BaseType >  Super; \
        \
        static bool Transfer(\
            typename Super::StringType& strRef,\
            const typename Super::FormatPattern& pattern,\
            Type arg) \
        { \
            typedef ImplType<TCharType, Type> ImplementationType; /*NOLINT(bugprone-macro-parentheses)*/ \
            return ImplementationType::Transfer(strRef, pattern, arg); \
        } \
    }

        FL_CONVERT_TRANSLATOR(int16_t, int16_t, TTranslatorBase, TIntegerTranslatorImpl);
        FL_CONVERT_TRANSLATOR(int32_t, int32_t, TTranslatorBase, TIntegerTranslatorImpl);
        FL_CONVERT_TRANSLATOR(long, int64_t, TTranslatorBase, TIntegerTranslatorImpl);
        FL_CONVERT_TRANSLATOR(uint8_t, uint8_t, TTranslatorBase, TIntegerTranslatorImpl);
        FL_CONVERT_TRANSLATOR(uint16_t, uint16_t, TTranslatorBase, TIntegerTranslatorImpl);
        FL_CONVERT_TRANSLATOR(uint32_t, uint32_t, TTranslatorBase, TIntegerTranslatorImpl);
        FL_CONVERT_TRANSLATOR(unsigned long, uint64_t, TTranslatorBase, TIntegerTranslatorImpl);

        FL_CONVERT_TRANSLATOR(long double, double, TTranslator, TDoubleTranslatorImpl);

#undef FL_CONVERT_TRANSLATOR

    }
}
