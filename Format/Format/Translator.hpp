// FormatLibrary 
// @author bodong 
// translator
#pragma once

namespace FormatLibrary
{
    template < typename TCharType, typename T >
    class TTranslatorBase
    {
    public:
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;        
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;        
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;
        
        template < typename TNumeric >
        static TNumeric Min( TNumeric x, TNumeric y )
        {
            return x < y?x:y;
        }

        static SIZE_T AdjustString(
            CharType* pszStr, 
            SIZE_T Length, 
            const SIZE_T BufLength, 
            const FormatPattern& Pattern
            )
        {
            SIZE_T UsedLength = Length;                       

            if (Pattern.HasWidth() && Pattern.Width > Length)
            {
                const SIZE_T PadCount = Min(BufLength - Length, Pattern.Width - Length);

                assert(PadCount < 0xFF);

                if (Pattern.Align == FormatPattern::AF_Left)
                {
                    // Fill at the end                    
                    CharTraits::Fill(pszStr + Length, ' ', PadCount);

                    pszStr[Length + PadCount] = 0;
                }
                else
                {
                    // fill at the start
                    CharTraits::copy(pszStr + PadCount, pszStr, Length);                                        
                    CharTraits::Fill(pszStr, ' ', PadCount);
                    pszStr[Length + PadCount] = 0;
                }

                UsedLength = Length + PadCount;
            }

            return UsedLength;
        }
    };

    template < typename TCharType >
    class TRawTranslator
    {
    public:
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef Utility::TAutoString<CharType>                      StringType;

        // Raw Translator always success
        static bool Transfer(StringType& S, const FormatPattern& Pattern, const CharType* Format)
        {
            assert(Pattern.Len > 0 && "invalid parameters!!!");

            S.AddStr(&Format[Pattern.Start], &Format[Pattern.Start] + Pattern.Len);            

            return true;
        }
    };

    template < typename TCharType, typename T >
    class TTranslator : public TTranslatorBase< TCharType, T >
    {
    protected:
        typedef TTranslatorBase< TCharType, T > Super;
        
        // if you get a compile error with this function can't visit
        // it means that you have transfer an unsupoorted parameter to Format pipeline
        // you can do them to fix this error:
        //    1. change your code, convert it to the support type
        //    2. make a specialization of TTranslator for your type.
        static bool Transfer(typename Super::StringType& S, const typename Super::FormatPattern& Pattern, const T& Arg);
    };

    template < typename TCharType >
    class TTranslator< TCharType, bool > :
        public TTranslatorBase< TCharType, bool >
    {
    public:
        typedef TTranslatorBase< TCharType, bool >                  Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;
        
        static bool Transfer(StringType& S, const FormatPattern& Pattern, bool Arg)
        {
            TCharType TrueStr[] = { 'T', 'r', 'u', 'e' };
            TCharType FalseStr[] = { 'F', 'a', 'l', 's', 'e' };

            const TCharType* Selection = Arg ? TrueStr : FalseStr;
            const SIZE_T Length = Arg ? _countof(TrueStr) : _countof(FalseStr);

            S.AddStr(Selection, Selection + Length);

            return true;
        }
    };

    enum
    {
        DEFAULT_FLOAT_PERCISION = 2
    };

    template < typename TCharType >
    class TTranslator< TCharType, FLOAT > :
    public TTranslatorBase< TCharType, FLOAT >
    {
    public:
        typedef TTranslatorBase< TCharType, FLOAT >                 Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;
        
        static bool Transfer(StringType& S, const FormatPattern& Pattern, FLOAT Arg)
        {
            CharType TempBuf[0xFF];

            if (Pattern.Flag == FormatPattern::FF_None ||
                Pattern.Flag == FormatPattern::FF_FixedPoint ||                
                Pattern.Flag == FormatPattern::FF_General
                )
            {
                SIZE_T Length = Algorithm::DoubleToString<CharType>(
                    Arg, 
                    TempBuf, 
                    _countof(TempBuf), 
                    Pattern.HasPrecision() ? Pattern.Precision : DEFAULT_FLOAT_PERCISION
                    );

                Length = Super::AdjustString(TempBuf, Length, _countof(TempBuf), Pattern);

                S.AddStr(TempBuf, TempBuf + Length);

                return true;
            }
            else if (Pattern.Flag == FormatPattern::FF_Decimal)
            {
                return TTranslator<TCharType, INT>::Transfer(S, Pattern, static_cast<INT>(Arg));
            }

            return false;
        }
    };    

    template < typename TCharType >
    class TTranslator< TCharType, INT64 > : 
        public TTranslatorBase< TCharType, INT64 >
    {
    public:
        typedef TTranslatorBase< TCharType, INT64 >                Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;
        
        static bool Transfer(StringType& S, const FormatPattern& Pattern, INT64 Arg)
        {
            CharType TempBuf[0xFF];

            if (Pattern.Flag == FormatPattern::FF_None ||
                Pattern.Flag == FormatPattern::FF_Decimal ||                
                Pattern.Flag == FormatPattern::FF_General ||
                Pattern.Flag == FormatPattern::FF_Hex
                )
            {
                const bool bHex = Pattern.Flag == FormatPattern::FF_Hex;

                SIZE_T Length = Algorithm::Int64ToString<CharType>(Arg, TempBuf, bHex?16:10);

                Length = Super::AdjustString(TempBuf, Length, _countof(TempBuf), Pattern);

                S.AddStr(TempBuf, TempBuf + Length);

                return true;
            }      
            else if (Pattern.Flag == FormatPattern::FF_FixedPoint)
            {
                return TTranslator<TCharType, FLOAT>::Transfer(S, Pattern, static_cast<FLOAT>(Arg));
            }

            return false;
        }
    };

    template < typename TCharType >
    class TTranslator< TCharType, UINT64 > : 
        public TTranslatorBase< TCharType, UINT64 >
    {
    public:
        typedef TTranslatorBase< TCharType, UINT64 >                Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;
        
        static bool Transfer(StringType& S, const FormatPattern& Pattern, UINT64 Arg)
        {
            CharType TempBuf[0xFF];

            if (Pattern.Flag == FormatPattern::FF_None ||
                Pattern.Flag == FormatPattern::FF_Decimal ||                
                Pattern.Flag == FormatPattern::FF_General ||
                Pattern.Flag == FormatPattern::FF_Hex
                )
            {
                const bool bHex = Pattern.Flag == FormatPattern::FF_Hex;

                SIZE_T Length = Algorithm::UInt64ToString<CharType>(Arg, TempBuf, bHex?16:10);

                Length = Super::AdjustString(TempBuf, Length, _countof(TempBuf), Pattern);

                S.AddStr(TempBuf, TempBuf + Length);

                return true;
            }      
            else if (Pattern.Flag == FormatPattern::FF_FixedPoint)
            {
                return TTranslator<TCharType, FLOAT>::Transfer(S, Pattern, static_cast<FLOAT>(Arg));
            }

            return false;
        }
    };

    template < typename TCharType >
    class TTranslator< TCharType, const TCharType* > :
        public TTranslatorBase< TCharType, const TCharType* >
    {
    public:
        typedef TTranslatorBase< TCharType, const TCharType* >      Super;
        typedef TCharType                                           CharType;
        typedef Algorithm::TFormatPattern<TCharType>                FormatPattern;
        typedef typename FormatPattern::ByteType                    ByteType;
        typedef typename FormatPattern::SizeType                    SizeType;
        typedef Utility::TAutoString<CharType>                      StringType;
        typedef Mpl::TCharTraits<CharType>                          CharTraits;
        
        static bool Transfer(StringType& S, const FormatPattern& /*Pattern*/, const TCharType* Str)
        {
            if (Str)
            {
                S.AddStr(Str);
            }            

            return true;
        }
    };

#define FL_CONVERT_TRNALSTOR( Type, BaseType ) \
    template < typename TCharType > \
    class TTranslator< TCharType, Type > : \
        public TTranslator< TCharType, BaseType > \
    { \
    public: \
        typedef TTranslator< TCharType, BaseType >  Super; \
        \
        static bool Transfer(typename Super::StringType& S, const typename Super::FormatPattern& Pattern, Type Arg) \
        { \
            return Super::Transfer(S, Pattern, static_cast<BaseType>(Arg)); \
        } \
    }

    FL_CONVERT_TRNALSTOR(INT, INT64);
    FL_CONVERT_TRNALSTOR(UINT, UINT64);
    FL_CONVERT_TRNALSTOR(char, INT);
    FL_CONVERT_TRNALSTOR(unsigned char, UINT);
    FL_CONVERT_TRNALSTOR(short, INT);
    FL_CONVERT_TRNALSTOR(unsigned short, UINT);

#if FL_COMPILER_MSVC
    FL_CONVERT_TRNALSTOR(DWORD, UINT64);
#elif FL_COMPILER_GCC
    FL_CONVERT_TRNALSTOR(unsigned long, UINT64);
#endif

#undef FL_CONVERT_TRNALSTOR
}

