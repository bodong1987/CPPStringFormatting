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
// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include <Format/Details/Translators.hpp>
#include <Format/Common/Mpl.hpp>

// ReSharper disable once CppEnforceNestedNamespacesStyle
namespace Formatting // NOLINT(*-concat-nested-namespaces)
{
    namespace Details
    {
        // calculate byte array hash code
        FL_CONSTEXPR14 inline size_t CalculateByteArrayHash(const uint8_t* const start, const size_t length)
        {
#if FL_PLATFORM_X64
            FL_STATIC_ASSERT(sizeof(void*) == 8, "This code is for 64-bit pointer.");

            constexpr size_t FNVOffsetBasis = 14695981039346656037ULL;

            // ReSharper disable once CppTooWideScope
            constexpr size_t FNVPrime = 1099511628211ULL;

#else
            FL_STATIC_ASSERT(sizeof(void*) == 4, "This code is for 32-bit pointer.");

            constexpr size_t FNVOffsetBasis = 2166136261U;

            // ReSharper disable once CppTooWideScope
            constexpr size_t FNVPrime = 16777619U;
#endif

            size_t Value = FNVOffsetBasis;
            size_t Next = 0;

            for (; Next + sizeof(size_t) <= length; Next += sizeof(size_t))
            {
                // fold in another byte
                Value ^= *reinterpret_cast<const size_t* const>(&start[Next]);
                Value *= FNVPrime;
            }

            // Process remaining bytes
            for (; Next < length; ++Next)
            {
                Value ^= static_cast<size_t>(start[Next]);
                Value *= FNVPrime;
            }

#if FL_PLATFORM_X64
            Value ^= Value >> 32;
#endif

            return Value;
        }

#if FL_COMPILER_IS_GREATER_THAN_CXX11
        // calculate constexpr string length
        //
        template < typename TCharType>
#if FL_COMPILER_IS_GREATER_THAN_CXX20
        consteval
#else
        FL_CONSTEXPR14
#endif
        inline size_t CalculateConstexprStringLength(const TCharType* str)
        {
#if FL_COMPILER_IS_GREATER_THAN_CXX17
            return std::char_traits<TCharType>::length(str);
#else
            size_t Count = 0;
            while (*str != TCharType())
            {
                ++Count;
                ++str;
            }

            return Count;
#endif // FL_COMPILER_IS_GREATER_THAN_CXX17
        }
#endif

#if FL_COMPILER_IS_GREATER_THAN_CXX11
        namespace Utils
        {
            template <typename TCharType, typename TPatternType, int32_t Index, typename T0, typename... T>
            struct DoTransferHelper
            {
                static bool DoTransfer(TAutoString<TCharType>& sink, const TPatternType& pattern, const TCharType* format, const T0& arg0, const T&... args)
                {
                    if (pattern.Index == Index)
                    {
                        typedef typename Mpl::IfElse<
                            Mpl::IsArray<T0>::Value,
                            const typename Mpl::RemoveArray<T0>::Type*,
                            T0
                        >::Type TransferType;

                        if (!TTranslator<TCharType, TransferType>::Transfer(sink, pattern, arg0))
                        {
                            TTranslator<TCharType, const TCharType*>::Transfer(sink, pattern, format);
                        }

                        return true;
                    }

                    return DoTransferHelper<TCharType, TPatternType, Index + 1, T...>::DoTransfer(sink, pattern, format, args...);
                }
            };

            template <typename TCharType, typename TPatternType, int32_t Index, typename T0>
            struct DoTransferHelper<TCharType, TPatternType, Index, T0>
            {
                static bool DoTransfer(TAutoString<TCharType>& sink, const TPatternType& pattern, const TCharType* format, const T0& arg0)
                {
                    if (pattern.Index == Index)
                    {
                        typedef typename Mpl::IfElse<
                            Mpl::IsArray<T0>::Value,
                            const typename Mpl::RemoveArray<T0>::Type*,
                            T0
                        >::Type TransferType;

                        /*
                        // if you get a compile error with Transfer function can't visit
                        // it means that you have transfer an unsupported parameter to format pipeline
                        // you can do them to fix this error:
                        //    1. change your code, convert it to the support type
                        //    2. make a specialization of TTranslator for your type.
                        */
                        if (!TTranslator<TCharType, TransferType>::Transfer(sink, pattern, arg0))
                        {
                            TTranslator<TCharType, const TCharType*>::Transfer(sink, pattern, format);
                        }

                        return true;
                    }

                    return false;
                }
            };

            template <typename TCharType, typename TPatternType, int32_t Index, typename T0, typename... T>
            inline bool DoTransfer(TAutoString<TCharType>& sink, const TPatternType& pattern, const TCharType* format, const T0& arg0, const T&... args)
            {
                return DoTransferHelper<TCharType, TPatternType, Index, T0, T...>::DoTransfer(sink, pattern, format, arg0, args...);
            }

            template <typename TCharType, typename TPatternType, int32_t Index>  // NOLINT
            inline bool DoTransfer(TAutoString<TCharType>& sink, const TPatternType& pattern, const TCharType* format)
            {
                return TRawTranslator<TCharType>::Transfer(sink, pattern, format);
            }
        }

        /// <summary>
        /// Formats to.
        /// format params to buffer
        /// </summary>
        /// <param name="sink">The sink.</param>
        /// <param name="patterns">patterns</param>
        /// <param name="format">The format.</param>
        /// <param name="length">format length</param>
        /// <param name="args">The arguments.</param>
        /// <returns>TAutoString&lt;TCharType&amp;.</returns>
        template <typename TCharType, typename TPatternListType, typename... T>
        inline TAutoString<TCharType>& FormatTo(TAutoString<TCharType>& sink, const TPatternListType* patterns, const TCharType* format, const size_t length, const T&... args)
        {
            if (patterns == nullptr)
            {
                sink.AddStr(format, length);

                return sink;
            }

            typename TPatternListType::ConstIterator Iter(*patterns);

            while (Iter.IsValid())
            {
                // ReSharper disable once CppTooWideScopeInitStatement
                const typename TPatternListType::ConstIterator::ValueType& Pattern = *Iter;

                if (Pattern.Flag == EFormatFlag::Raw)
                {
                    TRawTranslator<TCharType>::Transfer(sink, Pattern, format);
                }
                else
                {
                    if (!Utils::DoTransfer<TCharType, typename TPatternListType::ConstIterator::ValueType, 0, T...>(sink, Pattern, format, args...))
                    {
                        TRawTranslator<TCharType>::Transfer(sink, Pattern, format);
                    }
                }

                Iter.Next();
            }

            return sink;
        }

        /// <summary>
        /// Formats to.
        /// format params to buffer
        /// </summary>
        /// <param name="sink">The sink.</param>
        /// <param name="format">The format.</param>
        /// <param name="args">The arguments.</param>
        /// <returns>TAutoString&lt;TCharType&amp;.</returns>
        template <typename TCharType, typename TPatternStorageType, typename TFormatType, typename... T>
        inline TAutoString<TCharType>& FormatTo(TAutoString<TCharType>& sink, const TFormatType& format, const T&... args)
        {
            TPatternStorageType* Storage = TPatternStorageType::GetStorage();

            assert(Storage);

            // find patterns first
            const TCharType* localFormatText = Shims::PtrOf(format);
            const size_t localLength = Shims::LengthOf(format);

            const typename TPatternStorageType::PatternListType* Patterns = Storage->LookupPatterns(
                localFormatText,
                localLength,
                CalculateByteArrayHash(reinterpret_cast<const uint8_t*>(localFormatText), localLength*sizeof(TCharType))
                );

            assert(Patterns);

            return FormatTo<TCharType, typename TPatternStorageType::PatternListType, T...>(sink, Patterns, localFormatText, localLength, args...);
        }
#else
#define FL_FORMAT_TO_INDEX 0
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 1
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 2
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 3
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 4
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 5
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 6
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 7
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 8
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 9
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 10
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 11
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 12
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 13
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 14
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX

#define FL_FORMAT_TO_INDEX 15
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef FL_FORMAT_TO_INDEX
#endif

//
// If you want to support more parameters, you can continue to increase them here, but too large a number may cause the compiler to crash during compilation.
//
    }
}

