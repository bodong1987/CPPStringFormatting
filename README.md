CPPFormatLibrary
================

Fast CrossPlatform TypeSafe MultiThreadSupport Reentrant Code Header Only C++ Format Libarary, see wiki page for more detail


Welcome to the CPPFormatLibrary wiki!

this simple project is a C++ format library, it use .net style to format string. if you have used C++/CLI or C#, you will be very familiar with this code:

string str;
StandardLibrary::FormatTo(str, "{0}--#--{1,8}--#--{2}", 100, -40.2f, " String ");
and then, str is:

100--#--  -40.20--#-- String
this library is a cross-platform libary, i have compiled it under windows, macos, linux with msvc, g++ and xcode darwin C++ with c++ 11 support or with out c++ 11 support. and in most case, it will be 2x fast than c sprintf and it is thread safe.

this project design like boost, it is a header only project. if you want to use it, you only have to include the main Format.hpp.

this is a test useage:

// Test
#include "Format.hpp"

#include "Format/ProgressTimer.hpp"

using namespace FormatLibrary;

#include <iostream>
#include <vector>
using namespace std;

void TestProfile()
{
    const int TEST_COUNT = 100000;

    {
        Profile::ProgressTimer Timer("FL");

        for (int i = 0; i < TEST_COUNT; ++i)
        {
            string str;
            StandardLibrary::FormatTo(str, "{0}--#--{1,8}--#--{2}", 100, -40.2f, " String ");
            StandardLibrary::FormatTo(str, "{0}--#--{1,8}--#--{1}", 100, -40.2f);
            StandardLibrary::FormatTo(str, "{0}--#--{1,8}--#--{3}", 100, -40.2f);
        }
    }

#if !FL_COMPILER_MSVC
#define sprintf_s sprintf
#endif

    {
        Profile::ProgressTimer Timer("CL");

        for (int i = 0; i < TEST_COUNT; ++i)
        {
            string str;
            char szBuf[64];
            sprintf_s(szBuf, "%d--#--%8.2f--#--%s", 100, -40.2f, " String ");
            str = szBuf;
            sprintf_s(szBuf, "%d--#--%8.2f--#--%f", 100, -40.2f, 0.0f);
            str = szBuf;
            sprintf_s(szBuf, "%d--#--%8.2f--#--%%f", 100, -40.2f);
            str = szBuf;
        }
    }
}

#if FL_PLATFORM_HAS_CPP11 && FL_COMPILER_MSVC
#include <thread>

void TestProfileMultiThread()
{
    std::thread t0( TestProfile );
    std::thread t1( TestProfile );
    std::thread t2( TestProfile );

    t0.join();
    t1.join();
    t2.join();
}
#endif

int main()
{
    StandardLibrary::STLGlobalPatternStorageA Storage;
    Utility::TAutoString<char> TestStr;

    const char* pszTest = "{0},xxxd{1:d2}={2,3:d2} !! {{}} {0,-5:d8}";
    Storage.LookupPatterns(pszTest, strlen(pszTest));

    std::string str;
    StandardLibrary::FormatTo(str, "test{0}", 10);

    StandardLibrary::FormatTo(str, "{0}", char('c'), short(2));

#if FL_COMPILER_MSVC
    StandardLibrary::FormatTo(str, "0x{0:x}", 100, DWORD(100));
#endif

    std::wstring wstr;
    StandardLibrary::FormatTo(wstr, L"Test{1}, {2:f4}, {0}, {0,4}", L" X ", 20, -10.005f);

    cout << str << endl;
    wcout << wstr << endl;

    TestProfile();

#if FL_PLATFORM_HAS_CPP11 && FL_COMPILER_MSVC
    TestProfileMultiThread();
#endif

    return 0;
}
the release output:

0x64
Test20, -10.0050,  X ,  X
10028 FLElapse:0.0896266
10028 CLElapse:0.270948
9432 FLElapse:0.115886
10176 FLElapse:0.127729
8908 FLElapse:0.129378
10176 CLElapse:0.329533
9432 CLElapse:0.344293
8908 CLElapse:0.345957
this project can be integrated to your project very easy, this is an example demonstrate that how to integrate it to Unreal Engine 3/4.

// File : UnFormat.h

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
    typedef FormatLibrary::System::CriticalSection                              CriticalSectionType;
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


// File: UnFormatPost.h

// add by bodong
#pragma once

namespace FormatLibrary
{
    template <>
    class TTranslator< TCHAR, FString > :
        public TTranslatorBase< TCHAR, FString >
    {
    public:
        typedef TTranslatorBase< TCHAR, FString >  Super;

        static bool Transfer( Super::StringType& S, const Super::FormatPattern& /*Pattern*/, const FString& Arg)
        {
            if( !Arg.IsEmpty() )
            {
                S.AddStr( *Arg, *Arg + Arg.Len() );    
            }

            return true;
        }
    };

    template <>
    class TTranslator< TCHAR, FName > :
        public TTranslatorBase< TCHAR, FName >
    {
    public:
        typedef TTranslatorBase< TCHAR, FName >  Super;

        static bool Transfer( Super::StringType& S, const Super::FormatPattern& /*Pattern*/, const FName& Arg)
        {
            FString Temp;
            Arg.ToString(Temp);
            if( !Temp.IsEmpty() )
            {
                S.AddStr( *Temp, *Temp + Temp.Len() );                
            }

            return true;
        }
    };
}
// File: Readmet.txt
1.add UnFormat.h and UnFormatPost.h to Core/Inc
2.add #include "UnFormat.h" to the beginnng of UnString.h
3.add _IMPLEMENT_FORMAT_FOR_FSTRING(); to the public area for FString class content
4.add #include "UnFormatPost.h" after UnName.h
5.enjoy it!!
and then in UnrealEngine, FString have a static member function FormatTo and a normal member function Format, it can be used to format all kinds of types to FString linke FString::Printf.

in the main project, there is a integration of std::string and std::wstring, you can see it. enjoy this library. thank you very much.
