#pragma once

#include <Format/Details/Translators.hpp>
#include <Format/Common/Mpl.hpp>

namespace FormatLibrary
{
    namespace Details
    {
#if FL_COMPILER_WITH_CXX11
        namespace Utils
        {
            template <typename TCharType, typename TPatternType, typename T0, typename... T>
            struct DoTransferHelper
            {
                static bool DoTransfer(int32_t targetIndex, int32_t& currentIndex, TAutoString<TCharType>& sink, const TPatternType& pattern, const TCharType* format, const T0& arg0, const T&... args)
                {
                    if (targetIndex == currentIndex)
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

                    return DoTransferHelper<TCharType, TPatternType, T...>::DoTransfer(targetIndex, ++currentIndex, sink, pattern, format, args...);
                }
            };

            template <typename TCharType, typename TPatternType, typename T0>
            struct DoTransferHelper<TCharType, TPatternType, T0>
            {
                static bool DoTransfer(int32_t targetIndex, int32_t& currentIndex, TAutoString<TCharType>& sink, const TPatternType& pattern, const TCharType* format, const T0& arg0)
                {
                    if (targetIndex == currentIndex)
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

                    return false;
                }
            };

            template <typename TCharType, typename TPatternType, typename T0, typename... T>
            inline bool DoTransfer(int32_t targetIndex, int32_t& currentIndex, TAutoString<TCharType>& sink, const TPatternType& pattern, const TCharType* format, const T0& arg0, const T&... args)
            {
                return DoTransferHelper<TCharType, TPatternType, T0, T...>::DoTransfer(targetIndex, currentIndex, sink, pattern, format, arg0, args...);
            }

            template <typename TCharType, typename TPatternType>
            inline bool DoTransfer(int32_t targetIndex, int32_t& currentIndex, TAutoString<TCharType>& sink, const TPatternType& pattern, const TCharType* format)
            {
                return TRawTranslator<TCharType>::Transfer(sink, pattern, format);
            }
        }

        template <typename TCharType, typename TPatternStorageType, typename TFormatType, typename... T>
        inline TAutoString<TCharType>& FormatTo(TAutoString<TCharType>& sink, const TFormatType& format, const T&... args)
        {
            typedef typename TPatternStorageType::FormatPattern    FormatPatternType;
            typedef typename TPatternStorageType::PatternListType  PatternListType;
            typedef typename TPatternStorageType::PatternIterator  IteratorType;

            TPatternStorageType* Storage = TPatternStorageType::GetStorage();

            assert(Storage);

            const PatternListType* Patterns = Storage->LookupPatterns(Shims::PtrOf(format), Shims::LengthOf(format));

            assert(Patterns);

            if (Patterns == nullptr)
            {
                const TCharType* rawFormat = Shims::PtrOf(format);
                const size_t rawLength = Shims::LengthOf(format);
                sink.AddStr(rawFormat, rawFormat + rawLength);

                return sink;
            }

            IteratorType Iter(*Patterns);

            while (Iter.IsValid())
            {
                const FormatPatternType& Pattern = *Iter;

                if (Pattern.Flag == EFormatFlag::Raw)
                {
                    TRawTranslator<TCharType>::Transfer(sink, Pattern, Shims::PtrOf(format));
                }
                else
                {
                    int temp = 0;
                    if (!Utils::DoTransfer<TCharType, FormatPatternType, T...>(Pattern.Index, temp, sink, Pattern, Shims::PtrOf(format), args...))
                    {
                        TRawTranslator<TCharType>::Transfer(sink, Pattern, Shims::PtrOf(format));
                    }                    
                }

                Iter.Next();
            }

            return sink;
        }
#else
#define _FL_FORMAT_TO_INDEX_ 1
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 2
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 3
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 4
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 5
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 6
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 7
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 8
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 9
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 10
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 11
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 12
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 13
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 14
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 15
#include <Format/Details/InlineFiles/FormatTo.inl>
#undef _FL_FORMAT_TO_INDEX_
#endif

//
// If you want to support more parameters, you can continue to increase them here, but too large a number may cause the compiler to crash during compilation.
//
    }
}

