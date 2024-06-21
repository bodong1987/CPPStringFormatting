#pragma once

#include <string>
#include <Format/Details/FormatTo.hpp>
#include <Format/Details/StandardLibrary/StandardLibraryPolicy.hpp>

namespace FormatLibrary
{
    namespace Details
    {
        // make a specialization of std::basic_string
        template < typename TCharType >
        class TTranslator< TCharType, std::basic_string<TCharType> >
            : public TTranslatorBase< TCharType, std::basic_string<TCharType> >
        {
        public:
            typedef TTranslatorBase< TCharType, std::basic_string<TCharType> >      Super;

            static bool Transfer(typename Super::StringType& s, const typename Super::FormatPattern& /*Pattern*/, const std::basic_string<TCharType>& arg)
            {
                s.AddStr(arg.c_str(), arg.c_str() + arg.size());

                return true;
            }
        };
    }

    namespace StandardLibrary
    {
        // default FormatTo support Format with no arguments
        inline std::string& FormatTo(std::string& sink, const char* format)
        {
            if (format != nullptr)
            {
                sink.append(format);
            }

            return sink;
        }

        // default FormatTo support Format with no arguments
        inline std::wstring& FormatTo(std::wstring& sink, const wchar_t* format)
        {
            if (format != nullptr)
            {
                sink.append(format);
            }

            return sink;
        }

        template <typename TCharType>
        inline std::basic_string<TCharType> Format(const TCharType* format)
        {
            return format;
        }

        template <typename TCharType, typename T0, typename... T>
        inline std::basic_string<TCharType> Format(const TCharType* format, const T0& arg0, T... args)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage<Details::StandardLibrary::TStandardPolicy<TCharType>> GlobalPatternStorageType;
            
            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, const TCharType*, T0, T...>(Sink, format, arg0, args...);

            return Sink.CStr();
        }

        template <typename TCharType, typename T0, typename... T>
        inline std::basic_string<TCharType> Format(const std::basic_string<TCharType>& format, const T0& arg0, T... args)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage<Details::StandardLibrary::TStandardPolicy<TCharType>> GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, std::basic_string<TCharType>, T0, T...>(Sink, format, arg0, args...);

            return Sink.CStr();
        }
    }
}

