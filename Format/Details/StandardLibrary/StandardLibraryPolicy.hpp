#pragma once

#include <Format/Details/PatternStorage.hpp>

namespace FormatLibrary
{
    namespace Details
    {
        namespace StandardLibrary
        {
            template < typename TCharType >
            class TDefaultStringHasher
            {
            public:
                typedef TCharType                               CharType;
                typedef TFormatPattern<CharType>                FormatPattern;
                typedef typename FormatPattern::SizeType        SizeType;

                SizeType operator ()(const CharType* const start, SizeType length)
                {
                    const unsigned char* const StartTest = (const unsigned char* const)start;

                    SizeType Count = length * sizeof(CharType);

#if FL_PLATFORM_X64
                    static_assert(sizeof(SizeType) == 8, "This code is for 64-bit SizeType.");

                    const SizeType FNVOffsetBasis = 14695981039346656037ULL;
                    const SizeType FNVPrime = 1099511628211ULL;

#else
                    static_assert(sizeof(SizeType) == 4, "This code is for 32-bit SizeType.");

                    const SizeType FNVOffsetBasis = 2166136261U;
                    const SizeType FNVPrime = 16777619U;
#endif

                    SizeType Value = FNVOffsetBasis;

                    for (SizeType Next = 0; Next < Count; ++Next)
                    {
                        // fold in another byte
                        Value ^= (SizeType)StartTest[Next];
                        Value *= FNVPrime;
                    }

#if FL_PLATFORM_X64
                    static_assert(sizeof(SizeType) == 8, "This code is for 64-bit SizeType.");
                    Value ^= Value >> 32;

#else
                    static_assert(sizeof(SizeType) == 4, "This code is for 32-bit SizeType.");
#endif
                    return Value;
                }
            };

            template < typename TContainer >
            class TSTLConstIterator
            {
            public:
                typedef typename TContainer::const_iterator            ConstIterator;
                typedef typename ConstIterator::pointer                PointerType;
                typedef typename ConstIterator::reference              ReferenceType;

            public:
                TSTLConstIterator(const TContainer& con) :
                    ContainerRef(con),
                    CurrentIter(con.begin())
                {
                }

            public:
                inline bool       IsValid() const
                {
                    return CurrentIter != ContainerRef.end();
                }

                ReferenceType operator*() const
                {
                    return *CurrentIter;
                }

                inline bool       Next()
                {
                    assert(IsValid() && "Next Handle of Iterator Need Is Valid.");

                    ++CurrentIter;

                    return IsValid();
                }

            protected:
                const TContainer& ContainerRef;
                ConstIterator     CurrentIter;
            };

            template <typename TCharType>
            class TStandardPolicy
            {
            public:
                typedef TCharType                                              CharType;
                typedef TFormatPattern<CharType>                               FormatPattern;
                typedef typename FormatPattern::SizeType                       SizeType;
                typedef typename FormatPattern::ByteType                       ByteType;
                typedef TAutoArray<FormatPattern, 0xF, 0>                      PatternListType;
                typedef typename PatternListType::ConstIterator                PatternIterator;
                typedef std::unordered_map<SizeType, PatternListType>          PatternMapType;
                typedef TDefaultStringHasher<CharType>                         HasherType;
                typedef std::runtime_error                                     ExceptionType;

                static const PatternListType* FindByHashKey(const PatternMapType& storageReference, SizeType hashKey)
                {
                    typename PatternMapType::const_iterator itPos = storageReference.find(hashKey);

                    return itPos != storageReference.end() ? &itPos->second : nullptr;
                }

                static void ReserveList(PatternListType& /*ListRef*/, int /*Len*/)
                {
                    // AtuoArray does not need reserve
                }

                static const PatternListType* Emplace(PatternMapType& storageReference, SizeType hashKey, PatternListType&& patterns)
                {
                    std::pair< typename PatternMapType::iterator, bool> Results = storageReference.emplace(std::make_pair(hashKey, std::move(patterns)));

                    return Results.second ? &Results.first->second : nullptr;
                }

                static void AppendPattern(PatternListType& patterns, const FormatPattern& pattern)
                {
                    patterns.AddItem(pattern);
                }
            };
        }        
    }
}
