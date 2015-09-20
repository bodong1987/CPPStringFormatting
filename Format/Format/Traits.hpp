// FormatLibrary
// @author bodong
// traits
#pragma once

#if FL_COMPILER_GCC
#undef __STRICT_ANSI__
#include <cstdio>
#include <memory>
#include <string.h>
#include <cwchar>
#if FL_PLATFORM_HAS_CPP11
#include <type_traits>
#endif

#endif // FL_COMPILER_GCC

#include <utility>
#include <cstdarg>
#include <string>

namespace FormatLibrary
{
    namespace Mpl
    {
        template < typename TCharType >
        class TCharTraits :
            public std::char_traits<TCharType>
        {
        };

        template <>
        class TCharTraits<char> :
        public std::char_traits<char>
        {
        public:
            static INT StaticSprintf(
                char *string,
                SIZE_T sizeInBytes,
                const char *Format,
                ...
                )
            {
                va_list arglist;
                va_start(arglist, Format);

#if FL_COMPILER_MSVC
                return _vsprintf_s_l(string, sizeInBytes, Format, NULL, arglist);
#else
                return vsprintf(string, Format, arglist);
#endif
            }

            static const char* StaticEFormat()
            {
                return "%e";
            }

            static char* Fill(char* Dest, char Val, SIZE_T Length)
            {
                return (char*)memset(Dest, Val, Length);
            }
        };

        template <>
        class TCharTraits<wchar_t> :
        public std::char_traits<wchar_t>
        {
        public:
            static INT StaticSprintf(
                wchar_t *string,
                SIZE_T sizeInWords,
                const wchar_t *Format,
                ...
                )
            {
                va_list arglist;

                va_start(arglist, Format);

#if FL_COMPILER_MSVC
                return _vswprintf_s_l(string, sizeInWords, Format, NULL, arglist);
#elif FL_COMPILER_GCC && FL_PLATFORM_WINDOWS
                return vswprintf(string, Format, arglist);
#else
                return vswprintf(string, sizeInWords, Format, arglist);
#endif
            }

            static const wchar_t* StaticEFormat()
            {
                return L"%e";
            }

            static wchar_t* Fill(wchar_t* Dest, wchar_t Val, SIZE_T Length)
            {
                return (wchar_t*)wmemset(Dest, Val, Length);
            }
        };

        template <typename T>
        struct RemoveArray
        {
            typedef T Type;
        };

        template <typename T, SIZE_T N >
        struct RemoveArray< T[N]>
        {
            typedef T Type;
        };

        template <typename T, SIZE_T N >
        struct RemoveArray< const T[N]>
        {
            typedef T Type;
        };

        struct TrueType
        {
            enum { Value = 1 };
        };

        struct FalseType
        {
            enum { Value = 0 };
        };

        template < bool, typename T1, typename T2 >
        class TIfElse
        {
        public:
            typedef T2  Type;
        };

        template < typename T1, typename T2 >
        class TIfElse< true, T1, T2 >
        {
        public:
            typedef T1  Type;
        };               

        template < typename T1, typename T2 >
        struct IsSame : FalseType{};

        template < typename T >
        struct IsSame<T, T> : TrueType{};

        template < typename T >
        struct IsScalar : FalseType{};

#define FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE( Type ) \
        template <> \
        struct IsScalar< Type > : TrueType{}
        
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(bool);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(char);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(unsigned char);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(short);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(unsigned short);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(int);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(unsigned int);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(float);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(double);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(long double);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(INT64);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(UINT64);

        template < typename T >
        struct IsPtr : FalseType{};

        template < typename T >
        struct IsPtr<T*> : TrueType{};

        template < typename T >
        struct IsArray : FalseType{};

        template < typename T, INT Size >
        struct IsArray< T[Size] > : TrueType{};

        template < typename T >
        struct IsSimple
        {
            enum
            {
                Value =
#if FL_COMPILER_MSVC
#if _MSC_VER >= 1900  // after visual studio 2015
                std::is_trivially_copyable<T>::value
#else
                std::has_trivial_assign<T>::value
#endif

#elif FL_COMPILER_GCC                
#if (__GUNC__ >= 4 ) && (__GNUC_MINOR__ >= 3 )
                __has_trivial_assign(T)
#else
                IsScalar<T>::Value || IsPtr<T>::Value
#endif
#else
                std::is_trivially_copyable<T>::value
#endif
            };
        };
    }
}
