#pragma once

#include <Format/Details/Translators.hpp>
#include <Format/Common/Mpl.hpp>

namespace FormatLibrary
{
    namespace Details
    {
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
                sink.AddStr(Shims::PtrOf(format));
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
    }
}
