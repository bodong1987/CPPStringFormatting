// FormatLibrary
// @author bodong
// Pattern
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
            typedef std::size_t                                  SizeType;

            /// <summary>
            /// Enum EFormatFlag
            /// </summary>
            enum EFormatFlag
            {
                /// <summary>
                /// raw string
                /// </summary>
                FF_Raw,
                /// <summary>
                /// not defined
                /// </summary>
                FF_None,
                /// <summary>
                /// decimal
                /// </summary>
                FF_Decimal,
                /// <summary>
                /// exp
                /// </summary>
                FF_Exponent,
                /// <summary>
                /// fixed point
                /// </summary>
                FF_FixedPoint,
                /// <summary>
                /// general
                /// </summary>
                FF_General,
                /// <summary>
                /// CSV
                /// </summary>
                FF_CSV,
                /// <summary>
                /// percentage
                /// </summary>
                FF_Percentage,
                /// <summary>
                /// hexadecimal
                /// </summary>
                FF_Hex
            };

            /// <summary>
            /// Enum EAlignFlag
            /// </summary>
            enum EAlignFlag
            {
                /// <summary>
                /// right alignment
                /// </summary>
                AF_Right,
                /// <summary>
                /// left alignment
                /// </summary>
                AF_Left
            };

            /// <summary>
            /// Initializes a new instance of the <see cref="TFormatPattern"/> class.
            /// </summary>
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

            /// <summary>
            /// Gets the length.
            /// </summary>
            /// <returns>SizeType.</returns>
            SizeType  GetLength() const
            {
                return Len;
            }

            /// <summary>
            /// check if this pattern is valid
            /// </summary>
            /// <returns>bool.</returns>
            bool    IsValid() const
            {
                return Start != -1 && Len != -1 && Index >= 0;
            }

            /// <summary>
            /// Determines whether this instance has width.
            /// </summary>
            /// <returns>bool.</returns>
            bool    HasWidth() const
            {
                return Width != (ByteType)-1;
            }

            /// <summary>
            /// Determines whether this instance has precision.
            /// </summary>
            /// <returns>bool.</returns>
            bool    HasPrecision() const
            {
                return Precision != (ByteType)-1;
            }

        public:
            /// <summary>
            /// The start
            /// </summary>
            SizeType      Start;
            /// <summary>
            /// The length
            /// </summary>
            SizeType      Len;
            /// <summary>
            /// The flag
            /// </summary>
            EFormatFlag   Flag;
            /// <summary>
            /// The align
            /// </summary>
            EAlignFlag    Align;

            /// <summary>
            /// The index
            /// </summary>
            ByteType      Index;
            /// <summary>
            /// The precision
            /// </summary>
            ByteType      Precision;
            /// <summary>
            /// The width
            /// </summary>
            ByteType      Width;
        };
    }

    /// <summary>
    /// The Mpl namespace.
    /// </summary>
    namespace Mpl
    {
        /// <summary>
        /// make a partial specialization, so this TFormatPattern<TCharType> become a simple type
        /// it will have a fast Move and copy operation
        /// </summary>
        template < typename TCharType >
        struct IsSimple< Algorithm::TFormatPattern<TCharType> > : TrueType{};
    }
}
