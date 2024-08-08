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
                s.AddStr(arg.c_str(), arg.size());

                return true;
            }
        };

#if FL_COMPILER_IS_GREATER_THAN_CXX17
        // make a specialization of std::basic_string_view
        template < typename TCharType >
        class TTranslator< TCharType, std::basic_string_view<TCharType> >
            : public TTranslatorBase< TCharType, std::basic_string_view<TCharType> >
        {
        public:
            typedef TTranslatorBase< TCharType, std::basic_string_view<TCharType> >      Super;

            static bool Transfer(typename Super::StringType& s, const typename Super::FormatPattern& /*Pattern*/, const std::basic_string_view<TCharType>& arg)
            {
                s.AddStr(arg.data(), arg.size());

                return true;
            }
        };
#endif
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

        template <typename TCharType>
        inline std::basic_string<TCharType> Format(const typename Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType>::PatternListType* patterns, const TCharType* format, const size_t length)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType> > GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, typename GlobalPatternStorageType::PatternListType>(Sink, patterns, format, length);

            return std::basic_string<TCharType>(Sink.CStr(), Sink.GetLength());
        }

        template <typename TCharType, typename T0, typename... T>
        inline std::basic_string<TCharType> Format(const typename Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType>::PatternListType* patterns, const TCharType* format, const size_t length, const T0& arg0, T... args)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType> > GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, typename GlobalPatternStorageType::PatternListType, T0, T...>(Sink, patterns, format, length, arg0, args...);

            return std::basic_string<TCharType>(Sink.CStr(), Sink.GetLength());
        }

        template <typename TCharType>
        inline std::basic_string<TCharType>& FormatTo(std::basic_string<TCharType>& sink, const typename Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType>::PatternListType* patterns, const TCharType* format, const size_t length)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType> > GlobalPatternStorageType;

            sink.clear();

            SinkType Sink;
            Details::FormatTo<TCharType, typename GlobalPatternStorageType::PatternListType>(Sink, patterns, format, length);

            sink.assign(Sink.CStr(), Sink.GetLength());

            return sink;
        }

        template <typename TCharType, typename T0, typename... T>
        inline std::basic_string<TCharType>& FormatTo(std::basic_string<TCharType>& sink, const typename Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType>::PatternListType* patterns, const TCharType* format, const size_t length, const T0& arg0, T... args)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::StandardLibrary::TStandardPolicy<TCharType, Details::StandardLibrary::DefaultMutexType> > GlobalPatternStorageType;

            sink.clear();

            SinkType Sink;
            Details::FormatTo<TCharType, typename GlobalPatternStorageType::PatternListType, T0, T...>(Sink, patterns, format, length, arg0, args...);

            sink.assign(Sink.CStr(), Sink.GetLength());

            return sink;
        }
#else
#define FL_TEMPLATE_PARAMETERS_BODY( d, i ) \
    FL_PP_COMMA_IF(i) typename FL_PP_CAT(T, i)

#define FL_TEMPLATE_AGUMENT_BODY( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(T, i)

#define FL_NORMAL_AGUMENT_BODY( d, i ) \
    FL_PP_COMMA_IF(i) const FL_PP_CAT(T, i)& FL_PP_CAT(arg, i)

#define FL_REAL_ARGUMENT_BODY( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(arg, i)

#define FL_EXPORT_FOR_STRING( i ) \
    template <FL_PP_REPEAT(i, FL_TEMPLATE_PARAMETERS_BODY, ) > \
    std::string Format(const char* format, FL_PP_REPEAT(i, FL_NORMAL_AGUMENT_BODY, )) \
    { \
        TAutoString<char> Results; \
        Details::FormatTo<char, Details::StandardLibrary::STLGlobalPatternStorageA, const char*, FL_PP_REPEAT(i, FL_TEMPLATE_AGUMENT_BODY, )>(Results, format, FL_PP_REPEAT(i, FL_REAL_ARGUMENT_BODY, )); \
        return std::string(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, FL_TEMPLATE_PARAMETERS_BODY, ) > \
    std::wstring Format(const wchar_t* format, FL_PP_REPEAT(i, FL_NORMAL_AGUMENT_BODY, )) \
    { \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, Details::StandardLibrary::STLGlobalPatternStorageW, const wchar_t*, FL_PP_REPEAT(i, FL_TEMPLATE_AGUMENT_BODY, )>(Results, format, FL_PP_REPEAT(i, FL_REAL_ARGUMENT_BODY, )); \
        return std::wstring(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, FL_TEMPLATE_PARAMETERS_BODY, ) > \
    std::string Format(const std::string& format, FL_PP_REPEAT(i, FL_NORMAL_AGUMENT_BODY, )) \
    { \
        TAutoString<char> Results; \
        Details::FormatTo<char, Details::StandardLibrary::STLGlobalPatternStorageA, const char*, FL_PP_REPEAT(i, FL_TEMPLATE_AGUMENT_BODY, )>(Results, format.c_str(), FL_PP_REPEAT(i, FL_REAL_ARGUMENT_BODY, )); \
        return std::string(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, FL_TEMPLATE_PARAMETERS_BODY, ) > \
    std::wstring Format(const std::wstring& format, FL_PP_REPEAT(i, FL_NORMAL_AGUMENT_BODY, )) \
    { \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, Details::StandardLibrary::STLGlobalPatternStorageW, const wchar_t*, FL_PP_REPEAT(i, FL_TEMPLATE_AGUMENT_BODY, )>(Results, format.c_str(), FL_PP_REPEAT(i, FL_REAL_ARGUMENT_BODY, )); \
        return std::wstring(Results.CStr(), Results.GetLength()); \
    } \
    template < typename TFormatType, FL_PP_REPEAT(i, FL_TEMPLATE_PARAMETERS_BODY, ) > \
    void FormatTo(std::string& sink, const TFormatType& format, FL_PP_REPEAT(i, FL_NORMAL_AGUMENT_BODY, )) \
    { \
        sink.clear(); \
        TAutoString<char> Results; \
        Details::FormatTo< char, Details::StandardLibrary::STLGlobalPatternStorageA, const char*, FL_PP_REPEAT(i, FL_TEMPLATE_AGUMENT_BODY, )>(Results, Shims::PtrOf(format), FL_PP_REPEAT(i, FL_REAL_ARGUMENT_BODY, )); \
        sink.append(Results.CStr(), Results.GetLength()); \
    } \
    template < typename TFormatType, FL_PP_REPEAT(i, FL_TEMPLATE_PARAMETERS_BODY, ) > \
    void FormatTo(std::wstring& sink, const TFormatType& format, FL_PP_REPEAT(i, FL_NORMAL_AGUMENT_BODY, )) \
    { \
        sink.clear(); \
        TAutoString<wchar_t> Results; \
        Details::FormatTo< wchar_t, Details::StandardLibrary::STLGlobalPatternStorageW, const wchar_t*, FL_PP_REPEAT(i, FL_TEMPLATE_AGUMENT_BODY, )>(Results, Shims::PtrOf(format), FL_PP_REPEAT(i, FL_REAL_ARGUMENT_BODY, )); \
        sink.append(Results.CStr(), Results.GetLength()); \
    }

        // #pragma message( FL_PP_TEXT((FL_EXPORT_FOR_STRING(1))) )

        //        FL_EXPORT_FOR_STRING(1)

        FL_ENUM_SCALARS(FL_EXPORT_FOR_STRING);

#undef FL_TEMPLATE_PARAMETERS_BODY
#undef FL_TEMPLATE_AGUMENT_BODY
#undef FL_REAL_ARGUMENT_BODY
#undef FL_EXPORT_FOR_STRING
#undef FL_NORMAL_AGUMENT_BODY
#endif

#ifndef FL_DISABLE_STANDARD_LIBARY_MACROS
#if FL_COMPILER_IS_GREATER_THAN_CXX11
    #define FL_STD_FORMAT(format, ...) \
        Formatting::StandardLibrary::Format( \
            [](){ \
                typedef typename std::remove_const<typename std::remove_pointer<typename std::decay<decltype(format)>::type>::type>::type CharType; \
                static const auto S_Patterns = \
                    Formatting::Details::TPatternParser< \
                        Formatting::Details::StandardLibrary::TStandardPolicy< \
                            CharType, \
                            Formatting::Details::StandardLibrary::DefaultMutexType \
                        > \
                    >::Parse(format, Formatting::Shims::LengthOf(format)); \
                return &S_Patterns; \
            }(), \
            Formatting::Shims::PtrOf(format), \
            [](){ \
                FL_CONSTEXPR14 size_t hash = Formatting::Details::CalculateConstexprStringLength(format); \
                return hash; \
            }(), /*NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)*/\
            ##__VA_ARGS__ /*NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)*/\
            )

    #define FL_STD_FORMAT_TO(sink, format, ...) \
        Formatting::StandardLibrary::FormatTo( \
            sink, \
            [](){ \
                typedef typename std::remove_const<typename std::remove_pointer<typename std::decay<decltype(format)>::type>::type>::type CharType; \
                static const auto S_Patterns = \
                    Formatting::Details::TPatternParser< \
                        Formatting::Details::StandardLibrary::TStandardPolicy< \
                            CharType, \
                            Formatting::Details::StandardLibrary::DefaultMutexType \
                        > \
                    >::Parse(format, Formatting::Shims::LengthOf(format)); \
                return &S_Patterns; \
            }(), \
            Formatting::Shims::PtrOf(format), \
            [](){ \
                FL_CONSTEXPR14 size_t hash = Formatting::Details::CalculateConstexprStringLength(format); \
                return hash; \
            }(), /*NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)*/\
            ## __VA_ARGS__ /*NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)*/\
        )
#else
    #define FL_STD_FORMAT Formatting::StandardLibrary::Format
    #define FL_STD_FORMAT_TO Formatting::StandardLibrary::FormatTo
#endif
#endif
    }
}

