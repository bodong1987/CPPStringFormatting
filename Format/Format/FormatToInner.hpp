// FormatLibrary 
// @author bodong 

// Internal Use
#ifndef _FL_FORMAT_TO_INDEX_
#error "this is just be a internal file, don't include this file."
#endif

#define _FL_TEMPLATE_PARAMETERS_( d, i ) \
    FL_PP_COMMA_IF(i) typename FL_PP_CAT( T, i )

#define _FL_REAL_AGUMENTS_( d, i ) \
    FL_PP_COMMA_IF(i) const FL_PP_CAT(T,i)& FL_PP_CAT(Arg,i)

#define _FL_TRANSFER_BODY_( d, i ) \
    case i: \
        { \
            typedef FL_PP_CAT(T, i) Type; \
            typedef typename Mpl::TIfElse< \
                Mpl::IsArray<Type>::Value, \
                const typename Mpl::RemoveArray<Type>::Type*, \
                Type >::Type TransferType; \
                if (!TTranslator< TCharType, TransferType >::Transfer(Sink, Pattern, FL_PP_CAT(Arg, i))) \
            { \
                TRawTranslator< TCharType >::Transfer(Sink, Pattern, Format); \
            } \
        } \
        break;

//#pragma message( FL_PP_TEXT(FL_PP_REPEAT(_FL_FORMAT_TO_INDEX_, _FL_TEMPLATE_PARAMETERS_, )))

template < 
    typename TCharType,        
    typename TPatternStorageType,
    FL_PP_REPEAT(_FL_FORMAT_TO_INDEX_, _FL_TEMPLATE_PARAMETERS_, )
>
inline Utility::TAutoString<TCharType>& FormatTo(
    Utility::TAutoString<TCharType>& Sink,
    const TCharType* Format,
    FL_PP_REPEAT(_FL_FORMAT_TO_INDEX_, _FL_REAL_AGUMENTS_, )
)
{
    typedef typename TPatternStorageType::FormatPattern    FormatPatternType;
    typedef typename TPatternStorageType::PatternListType  PatternListType;
    typedef typename TPatternStorageType::PatternIterator  IteratorType;

    TPatternStorageType* Storage = TPatternStorageType::GetStorage();

    assert(Storage);

    const PatternListType* Patterns =
        Storage->LookupPatterns(Utility::PtrOf(Format), Utility::LengthOf(Format));

    assert(Patterns);

    IteratorType Iter(*Patterns);

    while (Iter.IsValid())
    {
        const FormatPatternType& Pattern = *Iter;

        if (Pattern.Flag == FormatPatternType::FF_Raw)
        {
            TRawTranslator<TCharType>::Transfer(Sink, Pattern, Format);
        }
        else
        {
            switch (Pattern.Index)
            {
                FL_PP_REPEAT(_FL_FORMAT_TO_INDEX_, _FL_TRANSFER_BODY_,);
            default:
                TRawTranslator<TCharType>::Transfer(Sink, Pattern, Format);
                break;
            }
        }

        Iter.Next();
    }

    return Sink;
}
        

#undef _FL_TEMPLATE_PARAMETERS_
#undef _FL_REAL_AGUMENTS_
#undef _FL_TRANSFER_BODY_