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
#include <Format/Details/StringConvertAlgorithm.hpp>
#include <Format/Details/Pattern.hpp>
#include <Format/Common/AutoString.hpp>
#include <Format/Common/Mpl.hpp>

namespace Formatting
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
            static inline void AppendString(StringType& strRef, const FormatPattern& pattern, const CharType* start, const SizeType length, CharType fillChar = CharTraits::GetSpace())
            {
                strRef.AddAlignStr(start, start + length, pattern.HasWidth() ? pattern.Width : static_cast<int>(length), pattern.Align != EAlignFlag::Left, fillChar);
            }

            static inline void AppendString(StringType& strRef, const FormatPattern& pattern, const CharType* start, const SizeType length, const int alignSize, bool paddingLeft, CharType fillChar = CharTraits::GetSpace())
            {
                FL_UNREFERENCED_PARAMETER(pattern);
                
                strRef.AddAlignStr(start, start + length, alignSize, paddingLeft, fillChar);
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

                strRef.AddStr(&format[pattern.Start], &format[pattern.Start] + pattern.Len);

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

            static bool TransferCore(StringType& strRef, const FormatPattern& pattern, FormatFlagType usedFlag, double arg)
            {
                FL_UNREFERENCED_PARAMETER(usedFlag);
                
                if (pattern.Flag == EFormatFlag::None ||
                    pattern.Flag == EFormatFlag::FixedPoint ||
                    pattern.Flag == EFormatFlag::General
                    )
                {
                    CharType TempBuf[64];

                    SizeType length = DoubleToString<CharType>(
                        arg,
                        TempBuf,
                        FL_ARRAY_COUNTOF(TempBuf),
                        pattern.HasPrecision() ? pattern.Precision : (pattern.Flag == EFormatFlag::FixedPoint ? DefaultFixedPointPrecision : DefaultPrecision) // NOLINT
                    );

                    Super::AppendString(strRef, pattern, TempBuf, length);

                    return true;
                }
                else if (pattern.Flag == EFormatFlag::Exponent)
                {
                    const CharType eFlagChar = pattern.bUpper ? 'E' : 'e';
                    CharType FmtBuf[16] = { '%', '.' };
                    const size_t length = Int64ToString(pattern.HasPrecision() ? pattern.Precision : DefaultExponentPrecision, FmtBuf + 2, 10, pattern.bUpper);
                    FmtBuf[length + 2] = eFlagChar;

                    CharType TempBuf[64];
                    size_t bufLength = CharTraits::StringPrintf(TempBuf, FL_ARRAY_COUNTOF(TempBuf), FmtBuf, arg);

                    const CharType* plusPos = CharTraits::rFind(TempBuf, eFlagChar);
                    if (plusPos != nullptr)
                    {
                        const CharType* endPos = TempBuf + bufLength;
                        const int exponentLength = static_cast<int>(endPos - plusPos);

                        if (exponentLength < DefaultMinExponentLength)
                        {
                            const int padLength = DefaultMinExponentLength - exponentLength;

                            memmove((void*)(plusPos + padLength + 2), plusPos + 2, exponentLength * sizeof(CharType)); // NOLINT

                            CharTraits::Fill(const_cast<CharType*>(plusPos + 2), CharTraits::GetZero(), padLength);

                            bufLength += padLength;
                        }
                    }

                    Super::AppendString(strRef, pattern, TempBuf, bufLength);

                    return true;
                }
                else if (pattern.Flag == EFormatFlag::Decimal)
                {
                    return TTranslator<TCharType, int64_t>::Transfer(strRef, pattern, static_cast<int64_t>(arg));
                }
                else if (pattern.Flag == EFormatFlag::Binary)
                {
                    assert(false && "double to binary string is not supported.");
                }

                return false;
            }

        public:
            static bool Transfer(StringType& strRef, const FormatPattern& pattern, double arg)
            {
                return TransferCore(strRef, pattern, pattern.Flag, arg);
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
        /// Class TInt64TranslatorImpl.
        /// Implements the <see cref="TTranslatorBase{TCharType, int64_t}" />
        /// </summary>
        /// <seealso cref="TTranslatorBase{TCharType, int64_t}" />
        template < typename TCharType, typename TRealType >
        class TInt64TranslatorImpl : public TTranslatorBase<TCharType, int64_t>
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
                if (pattern.Flag == EFormatFlag::None ||
                    pattern.Flag == EFormatFlag::Decimal ||
                    pattern.Flag == EFormatFlag::General ||
                    pattern.Flag == EFormatFlag::Hex
                    )
                {
                    const bool bHex = pattern.Flag == EFormatFlag::Hex;

                    CharType TempBuf[32];

                    SizeType length = Int64ToString<CharType>(arg, TempBuf, bHex ? 16 : 10, pattern.bUpper);

                    if (pattern.HasPrecision() && pattern.Precision > length)
                    {
                        Super::AppendString(strRef, pattern, TempBuf, length, pattern.Precision, true, CharTraits::GetZero());
                    }
                    else
                    {
                        Super::AppendString(strRef, pattern, TempBuf, length);
                    }

                    return true;
                }
                else if (pattern.Flag == EFormatFlag::Exponent)
                {
                    return TTranslator<TCharType, double>::Transfer(strRef, pattern, static_cast<double>(arg));
                }
                else if (pattern.Flag == EFormatFlag::FixedPoint)
                {
                    return TTranslator<TCharType, float>::Transfer(strRef, pattern, static_cast<float>(arg));
                }
                else if (pattern.Flag == EFormatFlag::Binary)
                {
                    constexpr int length = sizeof(TRealType) * 8;
                    CharType TempBuf[length + 1];

                    int usedLength = IntegerToBinaryString<TCharType, TRealType>(static_cast<TRealType>(arg), TempBuf);
                    const TCharType* textPos = TempBuf + (length - usedLength);

                    if (pattern.HasPrecision() && pattern.Precision > usedLength)
                    {
                        Super::AppendString(strRef, pattern, textPos, usedLength, pattern.Precision, true, CharTraits::GetZero());
                    }
                    else
                    {
                        Super::AppendString(strRef, pattern, textPos, usedLength);
                    }

                    return true;
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
                return TInt64TranslatorImpl<TCharType, int64_t>::Transfer(strRef, pattern, arg);
            }
        };

        /// <summary>
        /// Class TUInt64TranslatorImpl.
        /// Implements the <see cref="TTranslatorBase{TCharType, uint64_t}" />
        /// </summary>
        /// <seealso cref="TTranslatorBase{TCharType, uint64_t}" />
        template < typename TCharType, typename TRealType >
        class TUInt64TranslatorImpl : public TTranslatorBase<TCharType, uint64_t> // NOLINT
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
                if (pattern.Flag == EFormatFlag::None ||
                    pattern.Flag == EFormatFlag::Decimal ||
                    pattern.Flag == EFormatFlag::General ||
                    pattern.Flag == EFormatFlag::Hex
                    )
                {
                    const bool bHex = pattern.Flag == EFormatFlag::Hex;

                    CharType TempBuf[32];

                    SizeType length = UInt64ToString<CharType>(arg, TempBuf, bHex ? 16 : 10, pattern.bUpper);

                    if (pattern.HasPrecision() && pattern.Precision > length)
                    {
                        Super::AppendString(strRef, pattern, TempBuf, length, pattern.Precision, true, CharTraits::GetZero());
                    }
                    else
                    {
                        Super::AppendString(strRef, pattern, TempBuf, length);
                    }

                    return true;
                }
                else if (pattern.Flag == EFormatFlag::Exponent)
                {
                    return TTranslator<TCharType, double>::Transfer(strRef, pattern, static_cast<double>(arg));
                }
                else if (pattern.Flag == EFormatFlag::FixedPoint)
                {
                    return TTranslator<TCharType, float>::Transfer(strRef, pattern, static_cast<float>(arg));
                }
                else if (pattern.Flag == EFormatFlag::Binary)
                {
                    constexpr int length = sizeof(TRealType) * 8;
                    CharType TempBuf[length + 1];

                    int usedLength = IntegerToBinaryString<TCharType, TRealType>(static_cast<TRealType>(arg), TempBuf);
                    const TCharType* textPos = TempBuf + (length - usedLength);

                    if (pattern.HasPrecision() && pattern.Precision > usedLength)
                    {
                        Super::AppendString(strRef, pattern, textPos, usedLength, pattern.Precision, true, CharTraits::GetZero());
                    }
                    else
                    {
                        Super::AppendString(strRef, pattern, textPos, usedLength);
                    }

                    return true;
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

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, uint64_t arg)
            {
                return TUInt64TranslatorImpl<TCharType, uint64_t>::Transfer(strRef, pattern, arg);
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
                CharType TempBuf[32];

                const SizeType length = Details::UInt64ToString<CharType>(reinterpret_cast<uint64_t>(ptr), TempBuf, bHex ? 16 : 10, pattern.bUpper);

                if (pattern.HasPrecision() && pattern.Precision > length)
                {
                    Super::AppendString(strRef, pattern, TempBuf, length, pattern.Precision, true, CharTraits::GetZero());
                }
                else
                {
                    constexpr size_t defaultAlignedLength = sizeof(void*)*2;
                    Super::AppendString(strRef, pattern, TempBuf, length, defaultAlignedLength, true, CharTraits::GetZero());
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
#define FL_CONVERT_TRANSLATOR(Type, BaseType, ImplType) \
    template < typename TCharType > \
    class TTranslator< TCharType, Type > : \
        public TTranslator< TCharType, BaseType > \
    { \
    public: \
        typedef TTranslator< TCharType, BaseType >  Super; \
        \
        static bool Transfer(typename Super::StringType& strRef, const typename Super::FormatPattern& pattern, Type arg) \
        { \
            typedef ImplType<TCharType, Type> ImplementationType; /*NOLINT(bugprone-macro-parentheses)*/ \
            return ImplementationType::Transfer(strRef, pattern, arg); \
        } \
    }
        
        FL_CONVERT_TRANSLATOR(int16_t, int64_t, TInt64TranslatorImpl);
        FL_CONVERT_TRANSLATOR(int32_t, int64_t, TInt64TranslatorImpl);        
        FL_CONVERT_TRANSLATOR(long, int64_t, TInt64TranslatorImpl);
                
        FL_CONVERT_TRANSLATOR(uint8_t, uint64_t, TUInt64TranslatorImpl);        
        FL_CONVERT_TRANSLATOR(uint16_t, uint64_t, TUInt64TranslatorImpl);
        FL_CONVERT_TRANSLATOR(uint32_t, uint64_t, TUInt64TranslatorImpl);
        FL_CONVERT_TRANSLATOR(unsigned long, uint64_t, TUInt64TranslatorImpl);

        FL_CONVERT_TRANSLATOR(long double, double, TDoubleTranslatorImpl);

#undef FL_CONVERT_TRANSLATOR

    }
}
