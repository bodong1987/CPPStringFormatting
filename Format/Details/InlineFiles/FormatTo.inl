// FormatLibrary 
// @author bodong 

// Internal Use
#ifndef _FL_FORMAT_TO_INDEX_
#error "This is an internally used file, please do not include this file directly"
#endif

#if FL_COMPILER_WITH_CXX11
#error "This file is prepared to support versions of the C++ standard earlier than C++11. If your compiler already supports C++11 or newer, you should no longer use this set of code."
#endif

#define _FL_TEMPLATE_PARAMETERS_( d, i ) \
    FL_PP_COMMA_IF(i) typename FL_PP_CAT( T, i )

#define _FL_REAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) const FL_PP_CAT(T,i)& FL_PP_CAT(arg,i)

#define _FL_TRANSFER_BODY_( d, i ) \
    case i: \
        { \
        typedef FL_PP_CAT(T, i) Type; \
        typedef typename Mpl::IfElse< \
        Mpl::IsArray<Type>::Value, \
        const typename Mpl::RemoveArray<Type>::Type*, \
        Type >::Type TransferType; \
        if (!TTranslator< TCharType, TransferType >::Transfer(sink, Pattern, FL_PP_CAT(arg, i))) \
            { \
            TRawTranslator< TCharType >::Transfer(sink, Pattern, Shims::PtrOf(format)); \
            } \
        } \
        break;

//#pragma message( FL_PP_TEXT(FL_PP_REPEAT(_FL_FORMAT_TO_INDEX_, _FL_TEMPLATE_PARAMETERS_, )))

/*
* base iterate function 
* used to generate the real function for Format...
*/
template < 
typename TCharType,     
typename TPatternStorageType,
typename TFormatType,
FL_PP_REPEAT(_FL_FORMAT_TO_INDEX_, _FL_TEMPLATE_PARAMETERS_, )
>
inline TAutoString<TCharType>& FormatTo(
    TAutoString<TCharType>& sink,
    const TFormatType& format,
    FL_PP_REPEAT(_FL_FORMAT_TO_INDEX_, _FL_REAL_AGUMENTS_, )
    )
{
    typedef typename TPatternStorageType::FormatPattern    FormatPatternType;
    typedef typename TPatternStorageType::PatternListType  PatternListType;
    typedef typename TPatternStorageType::PatternIterator  IteratorType;

    TPatternStorageType* Storage = TPatternStorageType::GetStorage();

    assert(Storage);

    const PatternListType* Patterns =
        Storage->LookupPatterns(Shims::PtrOf(format), Shims::LengthOf(format));

    assert(Patterns);

    if (Patterns)
    {
        // walk though all patterns, and use TTranslator::Transfer to convert target type to string
        IteratorType Iter(*Patterns);

        while (Iter.IsValid())
        {
            const FormatPatternType& Pattern = *Iter;

            if (Pattern.Flag == EFormatFlag::Raw)
            {
                TRawTranslator<TCharType>::Transfer(sink, Pattern, Shims::PtrOf(format));
            }
            else
            {
                switch (Pattern.Index)
                {
                    FL_PP_REPEAT(_FL_FORMAT_TO_INDEX_, _FL_TRANSFER_BODY_, );
                default:
                    TRawTranslator<TCharType>::Transfer(sink, Pattern, Shims::PtrOf(format));
                    break;
                }
            }

            Iter.Next();
        }
    }
    else
    {
        const TCharType* rawFormat = Shims::PtrOf(format);
        const size_t rawLength = Shims::LengthOf(format);
        sink.AddStr(rawFormat, rawFormat + rawLength);
    }    

    return sink;
}


#undef _FL_TEMPLATE_PARAMETERS_
#undef _FL_REAL_AGUMENTS_
#undef _FL_TRANSFER_BODY_