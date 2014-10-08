// FormatLibrary 
// @author bodong 
// Reference
// http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
// http://www.drdobbs.com/flexible-c-1-efficient-integer-to-string/184401596?pgno=6
// https://code.google.com/p/stringencoders/downloads/detail?name=stringencoders-v3.10.3.tar.gz&can=2&q=
#pragma once

namespace FormatLibrary
{
    namespace Algorithm
    {
        template < typename TCharType >
        inline void StringReverse(TCharType* Start, TCharType* End)
        {
            TCharType Aux;

            while (Start < End)
            {
                Aux = *End;
                *End-- = *Start;
                *Start++ = Aux;
            }
        }

        namespace Detail
        {
            const char DigitMap[] = 
            { 
                '0', '1', '2', '3', '4', '5', '6',
                '7', '8', '9', 'A', 'B', 'C', 'D',
                'E', 'F'
            };
        }       

        template < typename TCharType >
        inline SIZE_T Int64ToString(INT64 Value, TCharType* Buf, INT Base)
        {
            assert(Base > 0 && static_cast<SIZE_T>(Base) <= _countof(Detail::DigitMap));

            TCharType* Str = Buf;

            UINT64 UValue = (Value < 0) ? -Value : Value;

            // Conversion. Number is reversed.
            do
            {
                *Str++ = Detail::DigitMap[UValue%Base];
            } while (UValue /= Base);

            if (Value < 0)
            {
                *Str++ = '-';
            }

            *Str = '\0';

            // Reverse string
            StringReverse<TCharType>(Buf, Str - 1);

            return Str - Buf;
        }

        template < typename TCharType >
        inline SIZE_T UInt64ToString(UINT64 Value, TCharType* Buf, INT Base)
        {
            assert(Base > 0 && static_cast<SIZE_T>(Base) <= _countof(Detail::DigitMap));

            TCharType* Str = Buf;

            // Conversion. Number is reversed.
            do
            {
                *Str++ = Detail::DigitMap[Value%Base];
            } while (Value /= Base);

            *Str = '\0';

            // Reverse string
            StringReverse<TCharType>(Buf, Str - 1);

            return Str - Buf;
        }

        template < typename TCharType >
        inline SIZE_T DoubleToString(DOUBLE Value, TCharType* Buf, SIZE_T Size, INT Precision)
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
            if (!(Value == Value))
            {
                Buf[0] = 'n'; Buf[1] = 'a'; Buf[2] = 'n'; Buf[3] = '\0';

                return 3;
            }

            /* if input is larger than ThresMax, revert to exponential */
            const DOUBLE ThresMax = (DOUBLE)(0x7FFFFFFF);

            DOUBLE Diff = 0.0;
            TCharType* Str = Buf;

            if (Precision < 0)
            {
                Precision = 0;
            }
            else if (Precision > 9)
            {
                /* Precision of >= 10 can lead to overflow errors */
                Precision = 9;
            }

            /* we'll work in positive values and deal with the
            negative sign issue later */
            INT Neg = 0;
            if (Value < 0)
            {
                Neg = 1;
                Value = -Value;
            }

            INT Whole = (INT)Value;
            DOUBLE tmp = (Value - Whole) * pow10[Precision];
            unsigned Frace = (unsigned)(tmp);

            Diff = tmp - Frace;

            if (Diff > 0.5)
            {
                ++Frace;
                /* handle rollover, e.g.  case 0.99 with Precision 1 is 1.0  */
                if (Frace >= pow10[Precision])
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

            if (Value > ThresMax)
            {
                INT Result = Mpl::TCharTraits<TCharType>::StaticSprintf(
                    Buf,
                    Size,
                    Mpl::TCharTraits<TCharType>::StaticEFormat(),
                    Neg ? -Value : Value);

                return Result;
            }

            if (Precision == 0)
            {
                Diff = Value - Whole;
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
                INT Count = Precision;
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

            StringReverse<TCharType>(Buf, Str - 1);

            return Str - Buf;
        }
    }
}