#pragma once

#include <Format/Common/Build.hpp>

#if FL_PLATFORM_WINDOWS && FL_COMPILER_MSVC
#include <afx.h>
#include <afxtempl.h>

#include <Format/Format.hpp>
#include <Format/Details/PatternStorage.hpp>

namespace Formatting
{
    namespace Details
    {
        namespace MFC
        {
            template < typename TCharType >
            class TDefaultStringHasher
            {
            public:
                typedef TCharType                               CharType;
                typedef TFormatPattern<CharType>                FormatPattern;
                typedef typename FormatPattern::SizeType        SizeType;

                SizeType operator ()(const CharType* const start, SizeType length)
                {
                    const unsigned char* const StartTest = (const unsigned char* const)start;

                    SizeType Count = length * sizeof(CharType);

#if FL_PLATFORM_X64
                    FL_STATIC_ASSERT(sizeof(SizeType) == 8, "This code is for 64-bit SizeType.");

                    const SizeType FNVOffsetBasis = 14695981039346656037ULL;
                    const SizeType FNVPrime = 1099511628211ULL;

#else
                    FL_STATIC_ASSERT(sizeof(SizeType) == 4, "This code is for 32-bit SizeType.");

                    const SizeType FNVOffsetBasis = 2166136261U;
                    const SizeType FNVPrime = 16777619U;
#endif

                    SizeType Value = FNVOffsetBasis;

                    for (SizeType Next = 0; Next < Count; ++Next)
                    {
                        // fold in another byte
                        Value ^= (SizeType)StartTest[Next];
                        Value *= FNVPrime;
                    }

#if FL_PLATFORM_X64
                    FL_STATIC_ASSERT(sizeof(SizeType) == 8, "This code is for 64-bit SizeType.");
                    Value ^= Value >> 32;

#else
                    FL_STATIC_ASSERT(sizeof(SizeType) == 4, "This code is for 32-bit SizeType.");
#endif
                    return Value;
                }
            };

            template <typename TCharType, typename TMutexType>
            class TMFCPolicy
            {
            public:
                typedef TCharType                                              CharType;
                typedef TFormatPattern<CharType>                               FormatPattern;
                typedef typename FormatPattern::SizeType                       SizeType;
                typedef typename FormatPattern::ByteType                       ByteType;
                typedef TAutoArray<FormatPattern, 0xF, 0>                      PatternListType;
                typedef typename PatternListType::ConstIterator                PatternIterator;
                typedef TDefaultStringHasher<CharType>                         HasherType;
                typedef std::runtime_error                                     ExceptionType;
                typedef TMutexType                                             MutexType;

                typedef CMap<SizeType, SizeType, PatternListType, PatternListType&>  PatternMapType;

                static const PatternListType* FindByHashKey(const PatternMapType& storageReference, SizeType hashKey)
                {
                    const PatternMapType::CPair* pairPtr = storageReference.PLookup(hashKey);

                    return pairPtr != nullptr ? &pairPtr->value : nullptr;
                }

                static void ReserveList(PatternListType& /*ListRef*/, int /*Len*/)
                {
                    // AtuoArray does not need reserve
                }

                static const PatternListType* Emplace(
                    PatternMapType& storageReference,
                    SizeType hashKey,
#if FL_COMPILER_HAS_RIGHT_VALUE_REFERENCE
                    PatternListType&& patterns
#else
                    const PatternListType& patterns
#endif
                )
                {
                    storageReference[hashKey] = FL_MOVE_SEMANTIC(patterns);
                    
                    return FindByHashKey(storageReference, hashKey);
                }

                static void AppendPattern(PatternListType& patterns, const FormatPattern& pattern)
                {
                    patterns.AddItem(pattern);
                }
            };

#if FL_WITH_THREAD_LOCAL || !FL_WITH_MULTITHREAD_SUPPORT
            typedef MutexNone                                       DefaultMutexType;
#else
            typedef Mutex                                           DefaultMutexType;
#endif

            typedef TPatternStorage< TMFCPolicy<char, DefaultMutexType> >          MFCPatternStorageA;
            typedef TPatternStorage< TMFCPolicy<wchar_t, DefaultMutexType> >       MFCPatternStorageW;
            typedef TGlobalPatternStorage< TMFCPolicy<char, DefaultMutexType> >    MFCGlobalPatternStorageA;
            typedef TGlobalPatternStorage< TMFCPolicy<wchar_t, DefaultMutexType> > MFCGlobalPatternStorageW;

#if FL_UNICODE
            typedef MFCPatternStorageW                                                  MFCPatternStorage;
            typedef MFCGlobalPatternStorageW                                            MFCGlobalPatternStorage;
#else
            typedef MFCPatternStorageA                                                  MFCPatternStorage;
            typedef MFCGlobalPatternStorageA                                            MFCGlobalPatternStorage;
#endif
        }
    }
}

namespace Formatting
{
    inline const TCHAR* PtrOf(const CString& str)
    {
        return str.GetString();
    }

    inline size_t LengthOf(const CString& str)
    {
        return str.GetLength();
    }

    namespace Details
    {
        // make a specialization of std::basic_string
        template <>
        class TTranslator<CString::XCHAR, CString>
            : public TTranslatorBase<CString::XCHAR, CString>
        {
        public:
            typedef TTranslatorBase<CString::XCHAR, CString>      Super;

            static bool Transfer(Super::StringType& s, const Super::FormatPattern& /*Pattern*/, const CString& arg)
            {
                s.AddStr(arg.GetString(), arg.GetString() + arg.GetLength());

                return true;
            }
        };
    }

    namespace MFC
    {
        template <typename TCharType>
        inline CString Format(const TCharType* format)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::MFC::TMFCPolicy<TCharType, Details::MFC::DefaultMutexType> >    GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, const TCharType*>(Sink, format);

            return CString(Sink.CStr(), Sink.GetLength());
        }

        // default FormatTo support Format with no arguments
        inline void FormatTo(CString& sink, const TCHAR* format)
        {
            sink = Format<TCHAR>(format);
        }

#if FL_COMPILER_WITH_CXX11
        template <typename TCharType, typename T0, typename... T>
        inline CString Format(const TCharType* format, const T0& arg0, T... args)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::MFC::TMFCPolicy<TCharType, Details::MFC::DefaultMutexType> >    GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, const TCharType*, T0, T...>(Sink, format, arg0, args...);

            return CString(Sink.CStr(), Sink.GetLength());
        }

        template <typename TCharType, typename T0, typename... T>
        inline CString Format(const CString& format, const T0& arg0, T... args)
        {
            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::MFC::TMFCPolicy<TCharType, Details::MFC::DefaultMutexType> >    GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, CString, T0, T...>(Sink, format, arg0, args...);

            return CString(Sink.CStr(), Sink.GetLength());
        }

        template <typename TCharType, typename TFormatType, typename T0, typename... T>
        inline void FormatTo(CString& sink, const TFormatType& format, const T0& arg0, T... args)
        {
            sink.Empty();

            typedef TAutoString<TCharType> SinkType;
            typedef Details::TGlobalPatternStorage< Details::MFC::TMFCPolicy<TCharType, Details::MFC::DefaultMutexType> >    GlobalPatternStorageType;

            SinkType Sink;
            Details::FormatTo<TCharType, GlobalPatternStorageType, TFormatType, T0, T...>(Sink, format, arg0, args...);

            sink.Append(Sink.CStr(), Sink.GetLength());
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
    CString Format(const TCHAR* format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<TCHAR> Results; \
        Details::FormatTo<TCHAR, Details::MFC::MFCGlobalPatternStorage, const TCHAR*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format, FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return CString(Results.CStr(), Results.GetLength()); \
    } \
    template <FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    CString Format(const CString& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        TAutoString<TCHAR> Results; \
        Details::FormatTo<TCHAR, Details::MFC::MFCGlobalPatternStorage, const TCHAR*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, format.c_str(), FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        return CString(Results.CStr(), Results.GetLength()); \
    } \
    template < typename TFormatType, FL_PP_REPEAT(i, _FL_TEMPLATE_PARAMETERS_, ) > \
    void FormatTo(CString& sink, const TFormatType& format, FL_PP_REPEAT(i, _FL_FORMAL_AGUMENTS_, )) \
    { \
        sink.Empty(); \
        TAutoString<TCHAR> Results; \
        Details::FormatTo< TCHAR, Details::MFC::MFCGlobalPatternStorage, const TCHAR*, FL_PP_REPEAT(i, _FL_TEMPLATE_AGUMENTS_, )>(Results, Shims::PtrOf(format), FL_PP_REPEAT(i, _FL_REAL_AGUMENTS_, )); \
        sink.Append(Results.CStr(), Results.GetLength()); \
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



#endif