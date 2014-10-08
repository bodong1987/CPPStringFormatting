// FormatLibrary
// @author bodong
// this is a simple wrapper for stl containers used for lookup patterns
#pragma once

#if FL_HAS_TR1_CONTAINERS
#include <unordered_map>
#else
#include <map>
#endif

#include <stdexcept>

namespace FormatLibrary
{
    namespace StandardLibrary
    {
        template < typename TCharType >
        class TDefaultStringHasher
        {
        public:
            typedef TCharType                               CharType;
            typedef Algorithm::TFormatPattern<CharType>     FormatPattern;
            typedef typename FormatPattern::SizeType        SizeType;

            SizeType operator ()(const CharType* const Start, SizeType Length)
            {
                const unsigned char* const StartTest = (const unsigned char* const)Start;

                SizeType Count = Length*sizeof(CharType);

#if FL_PLATFORM_X64
                FL_STATIC_ASSERT(sizeof(SizeType) == 8, "This code is for 64-bit SizeType.");

                const SizeType FNVOffsetBasis = 14695981039346656037ULL;
                const SizeType FNVPrime = 1099511628211ULL;

#else
                FL_STATIC_ASSERT(sizeof(SizeType) == 4, "This code is for 32-bit SizeType.");

                const SizeType FNVOffsetBasis = 2166136261U;
                const SizeType FNVPrime = 16777619U;
#endif

                SizeType Value = FNVOffsetBasis;

                for (SizeType Next = 0; Next < Count; ++Next)
                {	// fold in another byte
                    Value ^= (SizeType)StartTest[Next];
                    Value *= FNVPrime;
                }

#if FL_PLATFORM_X64
                FL_STATIC_ASSERT(sizeof(SizeType) == 8, "This code is for 64-bit SizeType.");
                Value ^= Value >> 32;

#else
                FL_STATIC_ASSERT(sizeof(SizeType) == 4, "This code is for 32-bit SizeType.");
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

        template <
            typename TCharType,
            typename TCirticalSectionType
        >
        class TStandardPolicy
        {
        public:
            typedef TCharType                                              CharType;
            typedef Algorithm::TFormatPattern<CharType>                    FormatPattern;
            typedef typename FormatPattern::SizeType                       SizeType;
            typedef typename FormatPattern::ByteType                       ByteType;
            typedef Utility::TAutoArray<FormatPattern,0xF,0>               PatternListType;
            typedef typename PatternListType::ConstIterator                PatternIterator;

#if FL_HAS_TR1_CONTAINERS
            typedef std::unordered_map<SizeType, PatternListType>          PatternMapType;
#else
            typedef std::map<SizeType, PatternListType>                    PatternMapType;
#endif
            typedef TCirticalSectionType                                   CriticalSectionType;
            typedef TDefaultStringHasher<CharType>                         HasherType;
            typedef std::runtime_error                                     ExceptionType;

            static const PatternListType* FindByHashKey(const PatternMapType& Storage, SizeType HashKey)
            {
                typename PatternMapType::const_iterator itPos = Storage.find(HashKey);

                return itPos != Storage.end() ? &itPos->second : NULL;
            }

            static void ReserveList(PatternListType& /*ListRef*/, int /*Len*/ )
            {
                // AtuoArray does not need reserve
            }

            static const PatternListType* Emplace(
                PatternMapType& Storage,
                SizeType HashKey,
#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
                PatternListType&& Patterns
#else
                const PatternListType& Patterns
#endif
                )
            {
                std::pair< typename PatternMapType::iterator, bool> Results =
#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
                    Storage.emplace(std::make_pair(HashKey, std::move(Patterns)));
#else
                    Storage.insert(std::make_pair(HashKey, Patterns));
#endif

                return Results.second ? &Results.first->second : NULL;
            }

            static void AppendPattern(PatternListType& Patterns, const FormatPattern& Pattern )
            {
                Patterns.AddItem(Pattern);
            }
        };
        
#if FL_WITH_THREAD_LOCAL || !FL_WITH_MULTITHREAD
        typedef Utility::CriticalSectionNone                    DefaultCritialSectionType;
#else
        typedef System::CriticalSection                         DefaultCritialSectionType;
#endif

        typedef Algorithm::TPatternStorage< TStandardPolicy<char, DefaultCritialSectionType> > STLPatternStorageA;
        typedef Algorithm::TPatternStorage< TStandardPolicy<wchar_t, DefaultCritialSectionType> > STLPatternStorageW;
        typedef Algorithm::TGlobalPatternStorage< TStandardPolicy<char, DefaultCritialSectionType> > STLGlobalPatternStorageA;
        typedef Algorithm::TGlobalPatternStorage< TStandardPolicy<wchar_t, DefaultCritialSectionType> > STLGlobalPatternStorageW;
    }
}

