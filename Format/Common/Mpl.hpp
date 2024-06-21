#pragma once

#include <Format/Common/Build.hpp>

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


namespace FormatLibrary
{
    namespace Mpl
    {
        template <typename T>
        struct RemoveArray
        {
            typedef T Type;
        };

        template <typename T, int N >
        struct RemoveArray< T[N]>
        {
            typedef T Type;
        };

        template <typename T, int N >
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
        class IfElse
        {
        public:
            typedef T2  Type;
        };

        template < typename T1, typename T2 >
        class IfElse< true, T1, T2 >
        {
        public:
            typedef T1  Type;
        };

        template < typename T1, typename T2 >
        struct IsSame : FalseType {};

        template < typename T >
        struct IsSame<T, T> : TrueType {};

        template < typename T >
        struct IsPtr : FalseType {};

        template < typename T >
        struct IsPtr<T*> : TrueType {};

        template < typename T >
        struct IsArray : FalseType {};

        template < typename T, int Size >
        struct IsArray< T[Size] > : TrueType {};

#if !FL_COMPILER_WITH_CXX11
        /// <summary>
        /// Struct IsScalar
        /// </summary>
        template < typename T >
        struct IsScalar : FalseType{};

#define FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE( Type ) \
    template <> \
        struct IsScalar< Type > : TrueType{}

        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(bool);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(int8_t);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(uint8_t);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(int16_t);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(uint16_t);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(int32_t);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(uint32_t);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(float);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(double);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(long double);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(int64_t);
        FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE(uint64_t);

#undef FL_PP_SPECIALIZATION_IS_SCALAR_TRUE_TYPE

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
#elif _MSC_VER >= 1600
                std::has_trivial_assign<T>::value
#else
                __has_trivial_assign(T)
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
#endif
    }
}
