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
				Width((ByteType)-1),
                bUpper(false)
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
            bool             bUpper;
        };
    }
}
