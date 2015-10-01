// created by bodong
#pragma once

#include <Format.hpp>

#include "Map.h"

class FUnrealFormatPolicy
{
public:
    typedef TCHAR                                                               CharType;
    typedef FormatLibrary::Algorithm::TFormatPattern<CharType>                  FormatPattern;
    typedef FormatPattern::SizeType                                             SizeType;
    typedef FormatPattern::ByteType                                             ByteType;
    typedef FormatLibrary::Utility::TAutoArray<FormatPattern,0xF,0>             PatternListType;
    typedef PatternListType::ConstIterator                                      PatternIterator;
    typedef TMap<SizeType, PatternListType>                                     PatternMapType; 

#if FL_WITH_THREAD_LOCAL || !FL_WITH_MULTITHREAD
    typedef FormatLibrary::Utility::CriticalSectionNone                         CriticalSectionType;
#else
    typedef FormatLibrary::System::CriticalSection                              CriticalSectionType;
#endif

    typedef FormatLibrary::StandardLibrary::TDefaultStringHasher<CharType>      HasherType;
    typedef std::runtime_error                                                  ExceptionType;

    static const PatternListType* FindByHashKey(const PatternMapType& Storage, SizeType HashKey)
    {        
        return Storage.Find(HashKey);
    }

    static void ReserveList(PatternListType& /*ListRef*/, int /*Len*/ )
    {
        // AtuoArray does not need reserve
    }

    static const PatternListType* Emplace(
        PatternMapType& Storage,
        SizeType HashKey,
#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
        PatternListType&& Patterns
#else
        const PatternListType& Patterns
#endif
        )
    {       
        return &Storage.Set( HashKey, MOVE_SEMANTIC(Patterns) );
    }

    static void AppendPattern(PatternListType& Patterns, const FormatPattern& Pattern )
    {
        Patterns.AddItem(Pattern);
    }
};

typedef FormatLibrary::Algorithm::TGlobalPatternStorage< FUnrealFormatPolicy > FGlobalPatternStorage;

#define _UE_FL_TEMPLATE_PARAMETERS_( d, i ) \
    FL_PP_COMMA_IF(i) typename FL_PP_CAT(T, i)

#define _UE_FL_TEMPLATE_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(T, i)

#define _UE_FL_FORMAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) const FL_PP_CAT(T, i)& FL_PP_CAT(Arg, i)

#define _UE_FL_REAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(Arg, i)

// make a export macro for FString
#define _UE_FL_EXPORT_FOR_STRING( i ) \
    template < FL_PP_REPEAT(i, _UE_FL_TEMPLATE_PARAMETERS_, ) > \
    static FString FormatTo(const TCHAR* Format, FL_PP_REPEAT(i, _UE_FL_FORMAL_AGUMENTS_, )) \
    { \
        check(Format); \
        FormatLibrary::Utility::TAutoString<TCHAR> Results; \
        FormatLibrary::Detail::FormatTo< TCHAR, FGlobalPatternStorage, FL_PP_REPEAT(i, _UE_FL_TEMPLATE_AGUMENTS_, )>(Results, Format, FL_PP_REPEAT(i, _UE_FL_REAL_AGUMENTS_, )); \
        FString Str( Results.GetLength(), Results.CStr() ); \
        return Str; \
    } \
    template < FL_PP_REPEAT(i, _UE_FL_TEMPLATE_PARAMETERS_, ) > \
    FString& Format( const TCHAR* Format, FL_PP_REPEAT(i, _UE_FL_FORMAL_AGUMENTS_, ) ) \
    { \
        *this = FL_MOVE_SEMANTIC( FString::FormatTo<FL_PP_REPEAT(i, _UE_FL_TEMPLATE_AGUMENTS_, )>( Format, FL_PP_REPEAT(i, _UE_FL_REAL_AGUMENTS_, ) ) ); \
        return *this; \
    }

#define _IMPLEMENT_FORMAT_FOR_FSTRING() \
    FL_ENUM_SCALARS(_UE_FL_EXPORT_FOR_STRING)


