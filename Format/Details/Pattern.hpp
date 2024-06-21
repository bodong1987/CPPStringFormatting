#pragma once

#include <Format/Common/Build.hpp>

namespace FormatLibrary
{
    namespace Details
    {
#if FL_COMPILER_WITH_CXX11
		enum class EFormatFlag : uint8_t
#else
		namespace EFormatFlag
		{ 
			enum Enum
#endif
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
#if FL_COMPILER_WITH_CXX11
		typedef EFormatFlag FormatFlagType;
#else
		}

		typedef EFormatFlag::Enum FormatFlagType;
#endif

#if FL_COMPILER_WITH_CXX11
        enum class EAlignFlag : uint8_t
#else
		namespace EAlignFlag
		{ 
			enum Enum
#endif
			{
				Right,
				Left
			};

#if FL_COMPILER_WITH_CXX11
		typedef EAlignFlag AlignFlagType;
#else
		}
		typedef EAlignFlag::Enum AlignFlagType;
#endif

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
                                                
			TFormatPattern() :
				Start((SizeType)-1),
				Len(0),
				Flag(EFormatFlag::Raw),
				Align(EAlignFlag::Right),
				Index((ByteType)-1),
				Precision((ByteType)-1),
				Width((ByteType)-1)
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
            SizeType         Start;
            SizeType         Len;            
            FormatFlagType   Flag;            
            AlignFlagType    Align;
            ByteType         Index;
            ByteType         Precision;
            ByteType         Width;
        };
    }
}
