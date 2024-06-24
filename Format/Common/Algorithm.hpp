/*
    MIT License

    Copyright (c) 2024 CppFormatLibrary

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

    Project URL: https://github.com/bodong1987/CPPFormatLibrary
*/
#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Common/Mpl.hpp>
#include <algorithm>

namespace Formatting
{
    namespace Algorithm
    {
#if !FL_COMPILER_WITH_CXX11
        namespace Details
        {
            /// <summary>
            /// Moves the array implementation.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="dest">The dest.</param>
            /// <param name="">The .</param>
            /// <returns>OutputType.</returns>
            template <typename InputType, typename OutputType >
            inline OutputType MoveArrayImpl(InputType start, InputType end, OutputType dest, Mpl::FalseType)
            {
                while (start != end)
                {
                    *dest = FL_MOVE_SEMANTIC(*start);
                    ++dest;
                    ++start;
                }

                return dest;
            }

            /// <summary>
            /// Moves the array implementation.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="dest">The dest.</param>
            /// <param name="">The .</param>
            /// <returns>T *.</returns>
            template <typename T >
            inline T* MoveArrayImpl(T* start, T* end, T* dest, Mpl::TrueType)
            {
                const size_t Size = (end - start)*sizeof(T);

                memcpy(dest, start, Size);

                return dest + (end - start);
            }

            /// <summary>
            /// Copies the array implementation.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="dest">The dest.</param>
            /// <param name="">The .</param>
            /// <returns>OutputType.</returns>
            template <typename InputType, typename OutputType >
            inline OutputType CopyArrayImpl(InputType start, InputType end, OutputType dest, Mpl::FalseType)
            {
                while (start != end)
                {
                    *dest = *start;
                    ++dest;
                    ++start;
                }

                return dest;
            }

            /// <summary>
            /// Copies the array implementation.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="end">The end.</param>
            /// <param name="dest">The dest.</param>
            /// <param name="">The .</param>
            /// <returns>T *.</returns>
            template <typename T >
            inline T* CopyArrayImpl(T* start, T* end, T* dest, Mpl::TrueType)
            {
                const size_t Size = (end - start)*sizeof(T);

                memcpy(dest, start, Size);

                return dest + (end - start);
            }
        }
#endif

        /// <summary>
        /// Moves the array.
        /// </summary>
        /// <param name="start">The start.</param>
        /// <param name="end">The end.</param>
        /// <param name="dest">The dest.</param>
        /// <returns>OutputType.</returns>
        template<typename InputType, typename OutputType>
        inline OutputType MoveArray(InputType start, InputType end, OutputType dest)
        {
#if !FL_COMPILER_WITH_CXX11
            typedef typename Mpl::IfElse<
                Mpl::IsSimple<InputType>::Value &&
                Mpl::IsSame<InputType, OutputType>::Value,
                Mpl::TrueType,
                Mpl::FalseType >::Type  Type;

            return Details::MoveArrayImpl(start, end, dest, Type());
#else
            return std::move<InputType, OutputType>(start, end, dest);
#endif
        }

        /// <summary>
        /// Copies the array.
        /// </summary>
        /// <param name="start">The start.</param>
        /// <param name="end">The end.</param>
        /// <param name="dest">The dest.</param>
        /// <returns>OutputType.</returns>
        template<typename InputType, typename OutputType>
        inline OutputType CopyArray(InputType start, InputType end, OutputType dest)
        {
#if !FL_COMPILER_WITH_CXX11
            typedef typename Mpl::IfElse<
                Mpl::IsSimple<InputType>::Value &&
                Mpl::IsSame<InputType, OutputType>::Value,
                Mpl::TrueType,
                Mpl::FalseType >::Type  Type;

            return Details::CopyArrayImpl(start, end, dest, Type());
#else
            return std::copy<InputType, OutputType>(start, end, dest);
#endif
        }

        /// <summary>
        /// Clamps the specified x.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="minValue">The minimum value.</param>
        /// <param name="maxValue">The maximum value.</param>
        /// <returns>T.</returns>
        template< class T > 
        constexpr inline T Clamp(const T x, const T minValue, const T maxValue)
        {
            return x < minValue ? minValue : x < maxValue ? x : maxValue;
        }

        /// <summary>
        /// Determines the maximum of the parameters.
        /// </summary>
        /// <param name="lhs">The LHS.</param>
        /// <param name="rhs">The RHS.</param>
        /// <returns>T.</returns>
        template< class T >
        constexpr inline T Max(const T lhs, const T rhs)
        {
            return lhs > rhs ? lhs : rhs;
        }

        /// <summary>
        /// Determines the minimum of the parameters.
        /// </summary>
        /// <param name="lhs">The LHS.</param>
        /// <param name="rhs">The RHS.</param>
        /// <returns>T.</returns>
        template < typename T >
        constexpr inline T Min(const T lhs, const T rhs)
        {
            return lhs < rhs ? lhs : rhs;
        }
    }
}