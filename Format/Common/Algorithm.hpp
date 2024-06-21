#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Common/Mpl.hpp>
#include <algorithm>

namespace FormatLibrary
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
        template< class T > inline T Clamp(const T x, const T minValue, const T maxValue)
        {
            return x < minValue ? minValue : x < maxValue ? x : maxValue;
        }
    }
}