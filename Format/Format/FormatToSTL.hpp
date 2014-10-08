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
    FL_PP_COMMA_IF(i) const FL_PP_CAT(T, i)& FL_PP_CAT(Arg, i)

#define _FL_REAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(Arg, i)

#define _FL_EXPORT_FOR_STRING( i ) \
    template < FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::string& FormatTo(std::string& Sink, const char* Format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
        { \
        Utility::TAutoString<char> Results; \
        Detail::FormatTo< char, STLGlobalPatternStorageA, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, Format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        Sink.clear(); \
        Sink.append(Results.CStr(), Results.GetLength()); \
        return Sink; \
    } \
    template < FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    std::wstring& FormatTo(std::wstring& Sink, const wchar_t* Format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        Utility::TAutoString<wchar_t> Results; \
        Detail::FormatTo< wchar_t, STLGlobalPatternStorageW, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, Format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        Sink.clear(); \
        Sink.append(Results.CStr(), Results.GetLength()); \
        return Sink; \
    }

// #pragma message( FL_PP_TEXT((_FL_EXPORT_FOR_STRING(1))) )
        
//        _FL_EXPORT_FOR_STRING(1)
    
    FL_ENUM_SCALARS(_FL_EXPORT_FOR_STRING);

#undef _FL_TEMPLATE_PARAMETERS_
#undef _FL_TEMPLATE_AGUMENTS_
#undef _FL_REAL_AGUMENTS_
#undef _FL_EXPORT_FOR_STRING
#undef _FL_FORMAL_AGUMENTS_
    }

    // make a specialization of std::basic_string
    template < typename TCharType >
    class TTranslator< TCharType, std::basic_string<TCharType> >
        : public TTranslatorBase< TCharType, std::basic_string<TCharType> >
    {
    public:
        typedef TTranslatorBase< TCharType, std::basic_string<TCharType> >      Super;

        static bool Transfer(typename Super::StringType& S, const typename Super::FormatPattern& /*Pattern*/, const std::basic_string<TCharType>& Arg)
        {
            S.AddStr(Arg.c_str(), Arg.c_str() + Arg.size());

            return true;
        }
    };
}