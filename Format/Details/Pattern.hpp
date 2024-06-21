#pragma once

namespace FormatLibrary
{
    namespace Details
    {
        enum class EFormatFlag
        {
            Raw,
            None,
            Decimal,
            Exponent,
            FixedPoint,
            General,
            CSV,
            Percentage,
            Hex
        };

        enum class EAlignFlag
        {
            Right,
            Left
        };

        /// <summary>
        /// Class TFormatPattern.
        /// This is the description of a Format unit
        /// example{ 0 } {0:d}
        /// </summary>
        template < typename TCharType >
        class TFormatPattern
        {
        public:
            typedef TCharType                                    CharType;
            typedef unsigned char                                ByteType;
            typedef size_t                                       SizeType;
                                                
            TFormatPattern()
            {
            }
                        
            SizeType  GetLength() const
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
            SizeType      Start     = (SizeType)-1;
            SizeType      Len       = 0;            
            EFormatFlag   Flag      = EFormatFlag::Raw;            
            EAlignFlag    Align     = EAlignFlag::Right;
            ByteType      Index     = (ByteType)-1;
            ByteType      Precision = (ByteType)-1;
            ByteType      Width     = (ByteType)-1;
        };
    }
}
