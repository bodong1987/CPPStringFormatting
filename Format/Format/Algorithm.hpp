// FormatLibrary 
// @author bodong 
// Reference
// http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
// http://www.drdobbs.com/flexible-c-1-efficient-integer-to-string/184401596?pgno=6
// https://code.google.com/p/stringencoders/downloads/detail?name=stringencoders-v3.10.3.tar.gz&can=2&q=
#pragma once

/// <summary>
/// The FormatLibrary namespace.
/// </summary>
namespace FormatLibrary
{
    /// <summary>
    /// The Algorithm namespace.
    /// </summary>
    namespace Algorithm
    {
        /// <summary>
        /// Strings the reverse.
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
        /// The Detail namespace.
        /// </summary>
        namespace Detail
        {
            /// <summary>
            /// The digit map
            /// provide fast index to hex number map
            /// </summary>
            const char DigitMap[] =
            { 
                '0', '1', '2', '3', '4', '5', '6',
                '7', '8', '9', 'A', 'B', 'C', 'D',
                'E', 'F'
            };
        }       

        /// <summary>
        /// fast Int64 to string.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="buffer">The buffer.</param>
        /// <param name="base">The base.</param>
        /// <returns>SIZE_T.</returns>
        template < typename TCharType >
        inline SIZE_T Int64ToString(INT64 value, TCharType* buffer, INT base)
        {
            assert(base > 0 && static_cast<SIZE_T>(base) <= _countof(Detail::DigitMap));

            TCharType* Str = buffer;

            UINT64 UValue = (value < 0) ? -value : value;

            // Conversion. Number is reversed.
            do
            {
                *Str++ = Detail::DigitMap[UValue%base];
            } while (UValue /= base);

            if (value < 0)
            {
                *Str++ = '-';
            }

            *Str = '\0';

            // Reverse string
            StringReverse<TCharType>(buffer, Str - 1);

            return Str - buffer;
        }

        /// <summary>
        /// fast uint64 to string.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="buffer">The buffer.</param>
        /// <param name="base">The base.</param>
        /// <returns>SIZE_T.</returns>
        template < typename TCharType >
        inline SIZE_T UInt64ToString(UINT64 value, TCharType* buffer, INT base)
        {
            assert(base > 0 && static_cast<SIZE_T>(base) <= _countof(Detail::DigitMap));

            TCharType* Str = buffer;

            // Conversion. Number is reversed.
            do
            {
                *Str++ = Detail::DigitMap[value%base];
            } while (value /= base);

            *Str = '\0';

            // Reverse string
            StringReverse<TCharType>(buffer, Str - 1);

            return Str - buffer;
        }

        /// <summary>
        /// fast Double to string.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="buffer">The buffer.</param>
        /// <param name="size">The size.</param>
        /// <param name="precision">The precision.</param>
        /// <returns>SIZE_T.</returns>
        template < typename TCharType >
        inline SIZE_T DoubleToString(DOUBLE value, TCharType* buffer, SIZE_T size, INT precision)
        {
            static const DOUBLE pow10[] =
            { 
                1, 10, 100, 1000, 10000, 100000, 1000000,
                10000000, 100000000, 1000000000 
            };

            /* Hacky test for NaN
            * under -fast-math this won't work, but then you also won't
            * have correct nan values anyways.  The alternative is
            * to link with libmath (bad) or hack IEEE DOUBLE bits (bad)
            */
            if (!(value == value))
            {
                buffer[0] = 'n'; buffer[1] = 'a'; buffer[2] = 'n'; buffer[3] = '\0';

                return 3;
            }

            /* if input is larger than ThresMax, revert to exponential */
            const DOUBLE ThresMax = (DOUBLE)(0x7FFFFFFF);

            DOUBLE Diff = 0.0;
            TCharType* Str = buffer;

            if (precision < 0)
            {
                precision = 0;
            }
            else if (precision > 9)
            {
                /* precision of >= 10 can lead to overflow errors */
                precision = 9;
            }

            /* we'll work in positive values and deal with the
            negative sign issue later */
            INT Neg = 0;
            if (value < 0)
            {
                Neg = 1;
                value = -value;
            }

            INT Whole = (INT)value;
            DOUBLE tmp = (value - Whole) * pow10[precision];
            unsigned Frace = (unsigned)(tmp);

            Diff = tmp - Frace;

            if (Diff > 0.5)
            {
                ++Frace;
                /* handle rollover, e.g.  case 0.99 with precision 1 is 1.0  */
                if (Frace >= pow10[precision])
                {
                    Frace = 0;
                    ++Whole;
                }
            }
            else if (Diff == 0.5 && ((Frace == 0) || (Frace & 1)))
            {
                /* if halfway, round up if odd, OR
                if last digit is 0.  That last part is strange */
                ++Frace;
            }

            /* for very large numbers switch back to native sprintf for exponentials.
            anyone want to write code to replace this? */
            /*
            normal printf behavior is to print EVERY Whole number digit
            which can be 100s of characters overflowing your buffers == bad
            */

            if (value > ThresMax)
            {
                INT Result = Mpl::TCharTraits<TCharType>::StaticSprintf(
                    buffer,
                    size,
                    Mpl::TCharTraits<TCharType>::StaticEFormat(),
                    Neg ? -value : value);

                return Result;
            }

            if (precision == 0)
            {
                Diff = value - Whole;
                if (Diff > 0.5)
                {
                    /* greater than 0.5, round up, e.g. 1.6 -> 2 */
                    ++Whole;
                }
                else if (Diff == 0.5 && (Whole & 1))
                {
                    /* exactly 0.5 and ODD, then round up */
                    /* 1.5 -> 2, but 2.5 -> 2 */
                    ++Whole;
                }
            }
            else
            {
                INT Count = precision;
                // now do fractional part, as an unsigned number
                do
                {
                    --Count;
                    *Str++ = (TCharType)(48 + (Frace % 10));
                } while (Frace /= 10);
                // add extra 0s
                while (Count-- > 0) *Str++ = '0';
                // add decimal
                *Str++ = '.';
            }

            // do Whole part
            // Take care of sign
            // Conversion. Number is reversed.
            do
            {
                *Str++ = (TCharType)(48 + (Whole % 10));
            } while (Whole /= 10);

            if (Neg)
            {
                *Str++ = '-';
            }

            *Str = '\0';

            StringReverse<TCharType>(buffer, Str - 1);

            return Str - buffer;
        }
    }
}