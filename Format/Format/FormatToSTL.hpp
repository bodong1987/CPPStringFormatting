// FormatLibrary 
// @author bodong 
// format to stl string
#pragma once

namespace FormatLibrary
{
    namespace StandardLibrary
    {
#define _FL_TEMPLATE_PARAMETERS_( d, i ) \
    FL_PP_COMMA_IF(i) typename FL_PP_CAT(T, i)

#define _FL_TEMPLATE_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(T, i)

#define _FL_FORMAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) const FL_PP_CAT(T, i)& FL_PP_CAT(arg, i)

#define _FL_REAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(arg, i)

#define _FL_EXPORT_FOR_STRING( i ) \
    template < FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::string& FormatTo(std::string& sink, const char* format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
        { \
        Utility::TAutoString<char> Results; \
        Detail::FormatTo< char, STLGlobalPatternStorageA, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        sink.clear(); \
        sink.append(Results.CStr(), Results.GetLength()); \
        return sink; \
    } \
    template < FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::wstring& FormatTo(std::wstring& sink, const wchar_t* format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        Utility::TAutoString<wchar_t> Results; \
        Detail::FormatTo< wchar_t, STLGlobalPatternStorageW, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
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

        // default FormatTo support Format with no arguments
	    inline std::string& FormatTo(std::string& sink, const char* format)
	    {
		    sink.append(format);
		    return sink;
	    }

        // default FormatTo support Format with no arguments
	    inline std::wstring& FormatTo(std::wstring& sink, const wchar_t* format)
	    {
		    sink.append(format);
		    return sink;
	    }

#if FL_PLATFORM_HAS_VARIADIC_TEMPLATE
        template <typename TCharType>
        inline std::basic_string<TCharType> Format(const TCharType* format)
        {
            return format;
        }

        template <typename TCharType, typename T0, typename... TArgumentTypes>
        inline std::basic_string<TCharType> Format(const TCharType* format, const T0& arg0, TArgumentTypes... args)
        {
            std::basic_string<TCHAR> target;
            FormatTo<T0, TArgumentTypes...>(target, format, arg0, args...);
            return target;
        }
#endif
    }

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