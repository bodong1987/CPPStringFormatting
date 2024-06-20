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
#if FL_COMPILER_WITH_CXX11
#include <type_traits>
#endif

#endif // FL_COMPILER_GCC

#include <utility>
#include <cstdarg>
#include <string>

#include <Format/Details/Types.hpp>

namespace FormatLibrary
{
    namespace Details
    {
        template < typename TCharType >
        class TCharTraits :
            public std::char_traits<TCharType>
        {
        };

        /// <summary>
        /// Class TCharTraits.
        /// Implements the <see cref="std::char_traits{char}" />
        /// </summary>
        /// <seealso cref="std::char_traits{char}" />
        template <>
        class TCharTraits<char> :
            public std::char_traits<char>
        {
        public:
            static INT StaticSprintf(
                char* string,
                SIZE_T sizeInBytes,
                const char* Format,
                ...
            )
            {
                va_list arglist;
                va_start(arglist, Format);

                int result = 0;
#if FL_COMPILER_MSVC
                result = _vsprintf_s_l(string, sizeInBytes, Format, NULL, arglist);
#else
                result = vsprintf(string, Format, arglist);
#endif
                va_end(arglist);

                return result;
            }

            static const char* StaticEFormat()
            {
                return "%e";
            }

            static char* Fill(char* Dest, char Val, SIZE_T Length)
            {
                return (char*)memset(Dest, Val, Length);
            }

            static int IsAlnum(char ch)
            {
                return isalnum(ch);
            }

            static int IsAlpha(char ch)
            {
                return isalpha(ch);
            }

            static int IsLower(char ch)
            {
                return islower(ch);
            }

            static int IsUpper(char ch)
            {
                return isupper(ch);
            }

            static int IsDigit(char ch)
            {
                return isdigit(ch);
            }

            static int IsXDigit(char ch)
            {
                return isxdigit(ch);
            }

            static int IsCntrl(char ch)
            {
                return iscntrl(ch);
            }

            static int IsGraph(char ch)
            {
                return isgraph(ch);
            }

            static int IsPrint(char ch)
            {
                return isprint(ch);
            }

            static int IsPunct(char ch)
            {
                return ispunct(ch);
            }

            static int IsSpace(char ch)
            {
                return isspace(ch);
            }

            static int ToLower(char ch)
            {
                return tolower(ch);
            }

            static int ToUpper(char ch)
            {
                return toupper(ch);
            }
        };

        template <>
        class TCharTraits<wchar_t> :
            public std::char_traits<wchar_t>
        {
        public:
            static INT StaticSprintf(
                wchar_t* string,
                SIZE_T sizeInWords,
                const wchar_t* Format,
                ...
            )
            {
                va_list arglist;

                va_start(arglist, Format);

                int result = 0;

#if FL_COMPILER_MSVC
                result = _vswprintf_s_l(string, sizeInWords, Format, NULL, arglist);
#elif FL_COMPILER_GCC && FL_PLATFORM_WINDOWS
                result = vswprintf(string, Format, arglist);
#else
                result = vswprintf(string, sizeInWords, Format, arglist);
#endif
                va_end(arglist);

                return result;
            }

            static const wchar_t* StaticEFormat()
            {
                return L"%e";
            }

            static wchar_t* Fill(wchar_t* Dest, wchar_t Val, SIZE_T Length)
            {
                return (wchar_t*)wmemset(Dest, Val, Length);
            }

            static int IsAlnum(wchar_t ch)
            {
                return iswalnum(ch);
            }

            static int IsAlpha(wchar_t ch)
            {
                return iswalpha(ch);
            }

            static int IsLower(wchar_t ch)
            {
                return iswlower(ch);
            }

            static int IsUpper(wchar_t ch)
            {
                return iswupper(ch);
            }

            static int IsDigit(wchar_t ch)
            {
                return iswdigit(ch);
            }

            static int IsXDigit(wchar_t ch)
            {
                return iswxdigit(ch);
            }

            static int IsCntrl(wchar_t ch)
            {
                return iswcntrl(ch);
            }

            static int IsGraph(wchar_t ch)
            {
                return iswgraph(ch);
            }

            static int IsPrint(wchar_t ch)
            {
                return iswprint(ch);
            }

            static int IsPunct(wchar_t ch)
            {
                return iswpunct(ch);
            }

            static int IsSpace(wchar_t ch)
            {
                return iswspace(ch);
            }

            static wchar_t ToLower(wchar_t ch)
            {
                return towlower(ch);
            }

            static wchar_t ToUpper(wchar_t ch)
            {
                return towupper(ch);
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
            /// <summary>
            /// The value
            /// </summary>
            enum { Value = 1 };
        };

        /// <summary>
        /// Struct FalseType
        /// </summary>
        struct FalseType
        {
            /// <summary>
            /// The value
            /// </summary>
            enum { Value = 0 };
        };

        /// <summary>
        /// Class TIfElse.
        /// </summary>
        template < bool, typename T1, typename T2 >
        class TIfElse
        {
        public:
            typedef T2  Type;
        };

        /// <summary>
        /// Class TIfElse.
        /// </summary>
        template < typename T1, typename T2 >
        class TIfElse< true, T1, T2 >
        {
        public:
            typedef T1  Type;
        };

        /// <summary>
        /// Struct IsSame
        /// </summary>
        template < typename T1, typename T2 >
        struct IsSame : FalseType {};

        /// <summary>
        /// Struct IsSame
        /// </summary>
        template < typename T >
        struct IsSame<T, T> : TrueType {};

        /// <summary>
        /// Struct IsScalar
        /// </summary>
        template < typename T >
        struct IsScalar : FalseType {};

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

        /// <summary>
        /// Struct IsPtr
        /// </summary>
        template < typename T >
        struct IsPtr : FalseType {};

        /// <summary>
        /// Struct IsPtr
        /// </summary>
        template < typename T >
        struct IsPtr<T*> : TrueType {};

        /// <summary>
        /// Struct IsArray
        /// </summary>
        template < typename T >
        struct IsArray : FalseType {};

        /// <summary>
        /// Struct IsArray
        /// </summary>
        template < typename T, INT Size >
        struct IsArray< T[Size] > : TrueType {};

        /// <summary>
        /// Struct IsSimple
        /// </summary>
        template < typename T >
        struct IsSimple
        {
            /// <summary>
            /// Enum __unnamed_enum_000e_3
            /// </summary>
            enum
            {
                /// <summary>
                /// The value
                /// </summary>
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

