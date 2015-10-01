// add by bodong
#pragma once

namespace FormatLibrary
{
    template <>
    class TTranslator< TCHAR, FString > :
        public TTranslatorBase< TCHAR, FString >
    {
    public:
        typedef TTranslatorBase< TCHAR, FString >  Super;

        static bool Transfer( Super::StringType& S, const Super::FormatPattern& /*Pattern*/, const FString& Arg)
        {
            if( !Arg.IsEmpty() )
            {
                S.AddStr( *Arg, *Arg + Arg.Len() );    
            }
            
            return true;
        }
    };

    template <>
    class TTranslator< TCHAR, FName > :
        public TTranslatorBase< TCHAR, FName >
    {
    public:
        typedef TTranslatorBase< TCHAR, FName >  Super;

        static bool Transfer( Super::StringType& S, const Super::FormatPattern& /*Pattern*/, const FName& Arg)
        {
            FString Temp;
            Arg.ToString(Temp);
            if( !Temp.IsEmpty() )
            {
                S.AddStr( *Temp, *Temp + Temp.Len() );                
            }

            return true;
        }
    };
}