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

#include <string>
#include <Format/Details/FormatTo.hpp>
#include <Format/Details/StandardLibrary/StandardLibraryPolicy.hpp>

namespace Formatting
{
    namespace Details // NOLINT
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
        // format with no params
        template <typename TCharType>
        inline std::basic_string<TCharType> Format(const TCharType* format)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType> >    GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, const TCharType*>(Sink, format);

            return std::basic_string<TCharType>(Sink.CStr(), Sink.GetLength());
        }

        // default FormatTo support Format with no arguments
        inline void FormatTo(std::string& sink, const char* format)
        {
            sink = Format<char>(format);
        }

        // default FormatTo support Format with no arguments
        inline void FormatTo(std::wstring& sink, const wchar_t* format)
        {
            sink = Format<wchar_t>(format);
        }

#if FL_COMPILER_IS_GREATER_THAN_CXX11
        template <typename TCharType, typename T0, typename... T>
        inline std::basic_string<TCharType> Format(const TCharType* format, const T0& arg0, T... args)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType> >    GlobalPatternStorageType;
            
            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, const TCharType*, T0, T...>(Sink, format, arg0, args...);

            return std::basic_string<TCharType>(Sink.CStr(), Sink.GetLength());
        }

        template <typename TCharType, typename T0, typename... T>
        inline std::basic_string<TCharType> Format(const std::basic_string<TCharType>& format, const T0& arg0, T... args)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType> >    GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, std::basic_string<TCharType>, T0, T...>(Sink, format, arg0, args...);

            return std::basic_string<TCharType>(Sink.CStr(), Sink.GetLength());
        }
                
        template <typename TCharType, typename TFormatType, typename T0, typename... T>
        inline void FormatTo(std::basic_string<TCharType>& sink, const TFormatType& format, const T0& arg0, T... args)
        {
            sink.clear();

            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType> >    GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, TFormatType, T0, T...>(Sink, format, arg0, args...);
                        
            sink.assign(Sink.CStr(), Sink.GetLength());
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
        Details::FormatTo<char, Details::StandardLibrary::STLGlobalPatternStorageA, const char*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return std::string(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::wstring Format(const wchar_t* format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, Details::StandardLibrary::STLGlobalPatternStorageW, const wchar_t*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return std::wstring(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::string Format(const std::string& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<char> Results; \
        Details::FormatTo<char, Details::StandardLibrary::STLGlobalPatternStorageA, const char*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format.c_str(), FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return std::string(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::wstring Format(const std::wstring& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, Details::StandardLibrary::STLGlobalPatternStorageW, const wchar_t*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format.c_str(), FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return std::wstring(Results.CStr(), Results.GetLength()); \
    } \
    template < typename TFormatType, FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    void FormatTo(std::string& sink, const TFormatType& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        sink.clear(); \
        TAutoString<char> Results; \
        Details::FormatTo< char, Details::StandardLibrary::STLGlobalPatternStorageA, const char*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, Shims::PtrOf(format), FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        sink.append(Results.CStr(), Results.GetLength()); \
    } \
    template < typename TFormatType, FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    void FormatTo(std::wstring& sink, const TFormatType& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        sink.clear(); \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, Details::StandardLibrary::STLGlobalPatternStorageW, const wchar_t*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, Shims::PtrOf(format), FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        sink.append(Results.CStr(), Results.GetLength()); \
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

