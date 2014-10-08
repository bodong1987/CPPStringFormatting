// FormatLibrary
// @author bodong
// Pattern
#pragma once

namespace FormatLibrary
{
    namespace Algorithm
    {
        /**
        * @brief This is the description of a Format unit
        * @example {0} {0:d}
        */
        template < typename TCharType >
        class TFormatPattern
        {
        public:
            typedef TCharType                                    CharType;
            typedef unsigned char                                ByteType;
            typedef std::size_t                                  SizeType;

            enum EFormatFlag
            {
                FF_Raw,
                FF_None,
                FF_Decimal,
                FF_Exponent,
                FF_FixedPoint,
                FF_General,
                FF_CSV,
                FF_Percentage,
                FF_Hex
            };

            enum EAlignFlag
            {
                AF_Right,
                AF_Left
            };

            TFormatPattern() :
                Start((SizeType)-1),
                Len(0),
                Flag(FF_Raw),
                Align(AF_Right),
                Index((ByteType)-1),
                Precision((ByteType)-1),
                Width((ByteType)-1)

            {
            }

            SizeType  GetLegnth() const
            {
                return Len;
            }

            bool    IsValid() const
            {
                return Start != -1 && Len != -1 && Index >= 0;
            }

            bool    HasWidth() const
            {
                return Width != (ByteType)-1;
            }

            bool    HasPrecision() const
            {
                return Precision != (ByteType)-1;
            }

        public:
            SizeType      Start;
            SizeType      Len;
            EFormatFlag   Flag;
            EAlignFlag    Align;

            ByteType      Index;
            ByteType      Precision;
            ByteType      Width;
        };
    }

    namespace Mpl
    {
        template < typename TCharType >
        struct IsSimple< Algorithm::TFormatPattern<TCharType> > : TrueType{};
    }
}
