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

#if FL_COMPILER_WITH_CXX11
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
#else
#define _FL_TEMPLATE_PARAMETERS_( d, i ) \
    FL_PP_COMMA_IF(i) typename FL_PP_CAT(T, i)

#define _FL_TEMPLATE_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(T, i)

#define _FL_FORMAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) const FL_PP_CAT(T, i)& FL_PP_CAT(arg, i)

#define _FL_REAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(arg, i)

#define _FL_EXPORT_FOR_STRING( i ) \
    template <FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::string Format(const char* format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<char> Results; \
        Details::FormatTo<char, const char*, Details::StandardLibrary::STLGlobalPatternStorageA, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return std::string(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::wstring Format(const wchar_t* format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, const wchar_t*, Details::StandardLibrary::STLGlobalPatternStorageW, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return std::wstring(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::string Format(const std::string& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<char> Results; \
        Details::FormatTo<char, std::string, Details::StandardLibrary::STLGlobalPatternStorageA, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return std::string(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::wstring Format(const std::wstring& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, std::wstring, Details::StandardLibrary::STLGlobalPatternStorageW, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return std::wstring(Results.CStr(), Results.GetLength()); \
    } \
    template < typename TFormatType, FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::string& FormatTo(std::string& sink, const TFormatType& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<char> Results; \
        Details::Format< char, Details::StandardLibrary::STLGlobalPatternStorageA, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        sink.clear(); \
        sink.append(Results.CStr(), Results.GetLength()); \
        return sink; \
    } \
    template < typename TFormatType, FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::wstring& Format(std::wstring& sink, const TFormatType& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, Details::StandardLibrary::STLGlobalPatternStorageW, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        sink.clear(); \
        sink.append(Results.CStr(), Results.GetLength()); \
        return sink; \
    }

        // #pragma message( FL_PP_TEXT((_FL_EXPORT_FOR_STRING(1))) )

        //        _FL_EXPORT_FOR_STRING(1)

        FL_ENUM_SCALARS(_FL_EXPORT_FOR_STRING);

#undef _FL_TEMPLATE_PARAMETERS_
#undef _FL_TEMPLATE_AGUMENTS_
#undef _FL_REAL_AGUMENTS_
#undef _FL_EXPORT_FOR_STRING
#undef _FL_FORMAL_AGUMENTS_
#endif
    }
}

