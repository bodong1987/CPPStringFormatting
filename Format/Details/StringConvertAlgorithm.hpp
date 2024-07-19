/*
    MIT License

    Copyright (c) 2024 CPPStringFormatting

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

    Project URL: https://github.com/bodong1987/CPPStringFormatting
*/
// References
// http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
// http://www.drdobbs.com/flexible-c-1-efficient-integer-to-string/184401596?pgno=6
// https://code.google.com/p/stringencoders/downloads/detail?name=stringencoders-v3.10.3.tar.gz&can=2&q=
#pragma once

#include <Format/Common/Build.hpp>
#include <Format/Common/CharTraits.hpp>
#include <Format/Common/Mpl.hpp>
#include "Format/Common/Algorithm.hpp"

namespace Formatting
{
    namespace Details
    {
        /// <summary>
        /// String reverse.
        /// </summary>
        /// <param name="start">The start.</param>
        /// <param name="end">The end.</param>
        template < typename TCharType >
        inline void StringReverse(TCharType* start, TCharType* end)
        {
            TCharType Aux;

            while (start < end)
            {
                Aux = *end;
                *end-- = *start;
                *start++ = Aux;
            }
        }

        /// <summary>
        /// Integers to binary string.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="buffer">The buffer.</param>
        template <typename TCharType, typename TIntegerType>
        inline int IntegerToBinaryString(TIntegerType value, TCharType buffer[sizeof(TIntegerType) * 8 + 1])
        {
            constexpr int length = sizeof(TIntegerType) * 8;

            buffer[length] = TCharTraits<TCharType>::GetEndFlag();

            for (int i = length-1; i >= 0; --i)
            {
                buffer[i] = (value&1) + TCharTraits<TCharType>::GetZero();

                value >>= 1;

                if (value == 0)
                {
                    return length - i;
                }
            }

            return length;
        }

        namespace  Utils
        {
            template <typename TCharType, typename TIntegerType, int32_t Base, bool IsSignedInteger>  // NOLINT
            class IntegerToStringHelper
            {
            };

            template <typename TCharType, typename TIntegerType, int32_t Base>
            class IntegerToStringHelper<TCharType, TIntegerType, Base, true>
            {
            public:
                inline static const TCharType* Convert(TIntegerType value, TCharType* const buffer, const size_t length, const bool upper)
                {
                    constexpr static char DigitMapUpper[] =
                    {
                        '0', '1', '2', '3', '4', '5', '6',
                        '7', '8', '9', 'A', 'B', 'C', 'D',
                        'E', 'F'
                    };

                    constexpr static char DigitMapLower[] =
                    {
                        '0', '1', '2', '3', '4', '5', '6',
                        '7', '8', '9', 'a', 'b', 'c', 'd',
                        'e', 'f'
                    };

                    FL_STATIC_ASSERT(Base > 0 && static_cast<size_t>(Base) <= FL_ARRAY_COUNTOF(DigitMapUpper), "Invalid operation");

                    const char* DigitMap = upper ? DigitMapUpper : DigitMapLower;
                    const bool IsNegativeNumber = value < 0;

                    TCharType* Str = buffer + length - 1;
                    *Str-- = TCharTraits<TCharType>::GetEndFlag();

                    typename Mpl::UnsignedTypeOf<TIntegerType>::Type UValue = IsNegativeNumber ? -value : value;

                    // Conversion. Number is reversed.
                    do
                    {
                        *Str-- = DigitMap[UValue % Base];
                    } while (UValue /= Base);

                    if (IsNegativeNumber)
                    {
                        *Str = '-';

                        return Str;
                    }

                    return Str + 1;
                }
            };

            template <typename TCharType, typename TIntegerType, int32_t Base>
            class IntegerToStringHelper<TCharType, TIntegerType, Base, false>
            {
            public:
                inline static const TCharType* Convert(TIntegerType value, TCharType* const buffer, const size_t length, const bool upper)
                {
                    constexpr static char DigitMapUpper[] =
                    {
                        '0', '1', '2', '3', '4', '5', '6',
                        '7', '8', '9', 'A', 'B', 'C', 'D',
                        'E', 'F'
                    };

                    constexpr static char DigitMapLower[] =
                    {
                        '0', '1', '2', '3', '4', '5', '6',
                        '7', '8', '9', 'a', 'b', 'c', 'd',
                        'e', 'f'
                    };

                    FL_STATIC_ASSERT(Base > 0 && static_cast<size_t>(Base) <= FL_ARRAY_COUNTOF(DigitMapUpper), "Invalid operation");

                    const char* DigitMap = upper ? DigitMapUpper : DigitMapLower;

                    TCharType* Str = buffer + length - 1;
                    *Str-- = TCharTraits<TCharType>::GetEndFlag();

                    // Conversion. Number is reversed.
                    do
                    {
                        *Str-- = DigitMap[value % Base];
                    } while (value /= Base);

                    return Str + 1;
                }
            };

        }

        /// <summary>
        /// Calculate the formated string length
        /// </summary>
        /// <param name="result">formated string text position</param>
        /// <param name="buffer">The buffer.</param>
        /// <param name="length">buffer length</param>
        /// <returns>return the result string start position</returns>
        template <typename TCharType>
        constexpr inline size_t CalculateConvertedStringLength(const TCharType* const result, const TCharType* const buffer, const size_t length)
        {
            return buffer + length - result - 1;
        }

        /// <summary>
        /// Calculate the formated string length
        /// </summary>
        /// <param name="result">formated string text position</param>
        /// <param name="buffer">The buffer.</param>
        /// <returns>return the result string start position</returns>
        template <typename TCharType, int32_t N>
        constexpr inline size_t CalculateConvertedStringLength(const TCharType* const result, const TCharType (&buffer)[N])
        {
            return buffer + N - result - 1;
        }

        /// <summary>
        /// Integer to string.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="buffer">The buffer.</param>
        /// <param name="length">buffer length</param>
        /// <param name="upper">The upper.</param>
        /// <returns>return the result string start position</returns>
        template < typename TCharType, typename TIntegerType, int32_t Base>
        inline const TCharType* IntegerToString(TIntegerType value, TCharType* const buffer, const size_t length, const bool upper)
        {
            return Utils::IntegerToStringHelper<TCharType, TIntegerType, Base, Mpl::IsSigned<TIntegerType>::Value>::Convert(
                    value,
                    buffer,
                    length,
                    upper
                );
        }

        /// <summary>
        /// Integer to string.
        /// the result position should be same with buffer
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="buffer">The buffer.</param>
        /// <param name="length">buffer length</param>
        /// <param name="upper">The upper.</param>
        /// <returns>return the result string start position</returns>
        template < typename TCharType, typename TIntegerType, int32_t Base>
        inline const TCharType* IntegerToStringMoved(TIntegerType value, TCharType* const buffer, const size_t length, const bool upper)
        {
            const TCharType* Result = IntegerToString<TCharType, TIntegerType, Base>(value, buffer, length, upper);
            const size_t ResultLength = CalculateConvertedStringLength(Result, buffer, length);

            memmove(buffer, Result, ResultLength * sizeof(TCharType));
            buffer[ResultLength] = TCharTraits<TCharType>::GetEndFlag();

            return buffer;
        }

        /// <summary>
        /// Doubles to string.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="buffer">The buffer.</param>
        /// <param name="length">The size.</param>
        /// <param name="precision">The precision.</param>
        /// <returns>the formatted string.</returns>
        template < typename TCharType >
        inline const TCharType* DoubleToString(double value, TCharType* buffer, const size_t length, int32_t precision)
        {
            constexpr static double Pow10[] =
            {
                1, 10, 100, 1000, 10000, 100000, 1000000,
                10000000, 100000000, 1000000000
            };

            /* Hacky test for NaN
            * under -fast-math this won't work, but then you also won't
            * have correct nan values anyway.  The alternative is
            * to link with libmath (bad) or hack IEEE double bits (bad)
            */
            if (!(value == value)) // NOLINT
            {
                TCharType* nanStr = buffer + length - 1;
                *nanStr-- = TCharTraits<TCharType>::GetEndFlag();
                *nanStr-- = 'n';
                *nanStr-- = 'a';
                *nanStr = 'n';

                return nanStr;
            }

            // check if it is a negative value
            const bool IsNegativeValue = value < 0;

            /* we'll work in positive values and deal with the
            negative sign issue later */
            if (IsNegativeValue)
            {
                value = -value;
            }

            /* if input is larger than ThresMax, revert to exponential */
            constexpr static double ThresMax = (double)(0x7FFFFFFF);  // NOLINT

            // ReSharper disable once CommentTypo
            /* for very large numbers switch back to native sprintf for exponentials.
                anyone want to write code to replace this? */
            /*
                normal printf behavior is to print EVERY Whole number digit
                which can be 100s of characters overflowing your buffers == bad
            */
            if (value > ThresMax)
            {
                // format by sprintf
                TCharType TempBuffer[64];
                const int32_t Result = TCharTraits<TCharType>::StringPrintf(
                    TempBuffer,
                    FL_ARRAY_COUNTOF(TempBuffer),
                    TCharTraits<TCharType>::StaticEFormat(),
                    IsNegativeValue ? -value : value
                    );

                TCharType* const TargetPos = buffer + length - (Result + 1);

                TCharTraits<TCharType>::copy(TargetPos, TempBuffer, Result + 1);

                return TargetPos;
            }

            // Ensure precision is within a reasonable range
            // precision of >= 10 can lead to overflow errors
            precision = Algorithm::Clamp(precision, 0, 9);

            // write reverse order
            TCharType* Str = buffer + length - 1;
            *Str-- = TCharTraits<TCharType>::GetEndFlag();

            // Get the integer part
            int32_t Whole = static_cast<int32_t>(value);

            // Calculate the fractional part
            const double tmp = (value - Whole) * Pow10[precision];

            // Get the fractional part
            uint32_t Frace = static_cast<uint32_t>(tmp);

            // Calculate the difference
            double DiffValue = tmp - Frace;

            // Handle rounding
            if (DiffValue > 0.5)
            {
                ++Frace;
                /* handle rollover, e.g.  case 0.99 with precision 1 is 1.0  */
                if (Frace >= Pow10[precision])
                {
                    Frace = 0;
                    ++Whole;
                }
            }
            // If exactly halfway, round up according to the rules
            else if (DiffValue == 0.5 && ((Frace == 0) || (Frace & 1))) // NOLINT
            {
                /* if halfway, round up if odd, OR
                if last digit is 0.  That last part is strange */
                ++Frace;
            }

            // Handle the case where precision is not 0
            if (precision != 0)
            {
                int32_t Count = precision;

                // now do fractional part, as an unsigned number
                do
                {
                    --Count;
                    *Str-- = static_cast<TCharType>(TCharTraits<TCharType>::GetZero() + (Frace % 10)); // NOLINT
                } while (Frace /= 10);

                // add extra 0s
                while (Count-- > 0) *Str-- = TCharTraits<TCharType>::GetZero();

                // add decimal
                *Str-- = '.';
            }
            else
            {
                DiffValue = value - Whole;
                if (DiffValue > 0.5)
                {   /*NOLINT(bugprone-branch-clone)*/
                    /* greater than 0.5, round up, e.g. 1.6 -> 2 */
                    ++Whole;
                }
                else if (DiffValue == 0.5 && (Whole & 1)) // NOLINT
                {
                    /* exactly 0.5 and ODD, then round up */
                    /* 1.5 -> 2, but 2.5 -> 2 */
                    ++Whole;
                }
            }

            // do Whole part
            // Take care of sign
            // Conversion. Number is reversed.
            do
            {
                *Str-- = static_cast<TCharType>(TCharTraits<TCharType>::GetZero() + (Whole % 10)); // NOLINT
            } while (Whole /= 10);

            if (IsNegativeValue)
            {
                *Str = '-';
                return Str;
            }

            return Str + 1;
        }

        /// <summary>
        /// Doubles to string.
        /// The result string should be same with buffer
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="buffer">The buffer.</param>
        /// <param name="length">The size.</param>
        /// <param name="precision">The precision.</param>
        /// <returns>the formatted string.</returns>
        template < typename TCharType >
        inline const TCharType* DoubleToStringMoved(double value, TCharType* buffer, const size_t length, int32_t precision)
        {
            const TCharType* Result = DoubleToString<TCharType>(value, buffer, length, precision);
            const size_t ResultLength = CalculateConvertedStringLength(Result, buffer, length);

            memmove(buffer, Result, ResultLength * sizeof(TCharType));
            buffer[ResultLength] = TCharTraits<TCharType>::GetEndFlag();

            return buffer;
        }
    }
}
