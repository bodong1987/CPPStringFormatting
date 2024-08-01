/*
    MIT License

    Copyright (c) 2024 CPPStringFormatting

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

    Project URL: https://github.com/bodong1987/CPPStringFormatting
*/

// ReSharper disable All
// Internal Use
#if FL_COMPILER_IS_GREATER_THAN_CXX11
#error "This file is prepared to support the C++ standard versions which are earlier than C++11. If your compiler already supports C++11 or newer, you should no longer use this set of code."
#endif

#ifdef FL_FORMAT_TO_INDEX
#define FL_TEMPLATE_PARAMETERS_BODY( d, i ) \
    FL_PP_COMMA_IF(i) typename FL_PP_CAT( T, i )

#define FL_TEMPLATE_PARAMETER_TYPE_BODY( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT( T, i )

#define FL_REAL_ARGUMENT_BODY( d, i ) \
    FL_PP_COMMA_IF(i) const FL_PP_CAT(T,i)& FL_PP_CAT(arg,i)

#define FL_REAL_ARGUMENT_ARG_BODY( d, i ) \
    FL_PP_COMMA_IF(i) FL_PP_CAT(arg,i)

#define FL_TRANSFER_BODY( d, i ) \
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

//#pragma message( FL_PP_TEXT(FL_PP_REPEAT(FL_FORMAT_TO_INDEX, FL_TEMPLATE_PARAMETERS_BODY, )))

// These two macros can reduce false positives during static code scanning
#ifndef FL_FORMAT_TO_INDEX
#define FL_PP_COMMA_IF(...) 
#define FL_PP_REPEAT(n,m,d)
#endif

/*
* base iterate function 
* used to generate the real function for Format...
* need provide pattern list...
*/
template < 
    typename TCharType,
    typename TPatternStorageType
    FL_PP_COMMA_IF(FL_FORMAT_TO_INDEX)
    FL_PP_REPEAT(FL_FORMAT_TO_INDEX, FL_TEMPLATE_PARAMETERS_BODY, )
>
inline TAutoString<TCharType>& FormatTo( 
    TAutoString<TCharType>& sink, 
    const typename TPatternStorageType::PatternListType* patterns,
    const TCharType* format,
    const size_t length
    FL_PP_COMMA_IF(FL_FORMAT_TO_INDEX)
    FL_PP_REPEAT(FL_FORMAT_TO_INDEX, FL_REAL_ARGUMENT_BODY, )
    )
{
    if (patterns == nullptr)
    {
        sink.AddStr(format, length);
        return sink;
    }

    // walk though all patterns, and use TTranslator::Transfer to convert target type to string
    typename TPatternStorageType::PatternIterator Iter(*patterns);

    while (Iter.IsValid())
    {
        const typename TPatternStorageType::FormatPattern& Pattern = *Iter;

        if (Pattern.Flag == EFormatFlag::Raw)
        {
            TRawTranslator<TCharType>::Transfer(sink, Pattern, Shims::PtrOf(format));
        }
        else
        {
            switch (Pattern.Index)
            {
                /*
                // if you get a compile error with Transfer function can't visit
                // it means that you have transfer an unsupported parameter to format pipeline
                // you can do them to fix this error:
                //    1. change your code, convert it to the support type
                //    2. make a specialization of TTranslator for your type.
                */
                FL_PP_REPEAT(FL_FORMAT_TO_INDEX, FL_TRANSFER_BODY, );
            default:
                TRawTranslator<TCharType>::Transfer(sink, Pattern, Shims::PtrOf(format));
                break;
            }
        }

        Iter.Next();
    } 

    return sink;
}


/*
* base iterate function 
* used to generate the real function for Format...
*/
template < 
    typename TCharType,
    typename TPatternStorageType,
    typename TFormatType
    FL_PP_COMMA_IF(FL_FORMAT_TO_INDEX)
    FL_PP_REPEAT(FL_FORMAT_TO_INDEX, FL_TEMPLATE_PARAMETERS_BODY, )
>
inline TAutoString<TCharType>& FormatTo( 
    TAutoString<TCharType>& sink, 
    const TFormatType& format
    FL_PP_COMMA_IF(FL_FORMAT_TO_INDEX)
    FL_PP_REPEAT(FL_FORMAT_TO_INDEX, FL_REAL_ARGUMENT_BODY, )
    )
{
    typedef typename TPatternStorageType::FormatPattern    FormatPatternType;
    typedef typename TPatternStorageType::PatternListType  PatternListType;
    typedef typename TPatternStorageType::PatternIterator  IteratorType;

    TPatternStorageType* Storage = TPatternStorageType::GetStorage();

    assert(Storage);

    const TCharType* localFormatText = Shims::PtrOf(format);
    const size_t localLength = Shims::LengthOf(format);
    
    const PatternListType* Patterns = Storage->LookupPatterns(
        localFormatText,
        localLength,
        CalculateByteArrayHash(reinterpret_cast<const uint8_t*>(localFormatText), localLength*sizeof(TCharType))
        );

    assert(Patterns);

    return FormatTo<
                TCharType, 
                TPatternStorageType
                FL_PP_COMMA_IF(FL_FORMAT_TO_INDEX)
                FL_PP_REPEAT(FL_FORMAT_TO_INDEX, FL_TEMPLATE_PARAMETER_TYPE_BODY, )
    >(
        sink, 
        Patterns, 
        localFormatText, 
        localLength
        FL_PP_COMMA_IF(FL_FORMAT_TO_INDEX)
        FL_PP_REPEAT(FL_FORMAT_TO_INDEX, FL_REAL_ARGUMENT_ARG_BODY, )
    );
}

#undef FL_REAL_ARGUMENT_ARG_BODY
#undef FL_TEMPLATE_PARAMETER_TYPE_BODY
#undef FL_TEMPLATE_PARAMETERS_BODY
#undef FL_REAL_ARGUMENT_BODY
#undef FL_TRANSFER_BODY
#else
#pragma message("This is an internally used file, please do not include this file directly")
#endif
// ReSharper restore All
