#pragma once

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
    }
}
