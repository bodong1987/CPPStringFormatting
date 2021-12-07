// FormatLibrary
// @author bodong
// this is a simple wrapper for stl containers used for lookup patterns
// default policy, you can provide your custom policy
#pragma once

#if FL_HAS_TR1_CONTAINERS
#include <unordered_map>
#else
#include <map>
#endif

#include <stdexcept>

/// <summary>
/// The FormatLibrary namespace.
/// </summary>
namespace FormatLibrary
{
    /// <summary>
    /// The StandardLibrary namespace.
    /// </summary>
    namespace StandardLibrary
    {
        /// <summary>
        /// Class TDefaultStringHasher.
        /// </summary>
        template < typename TCharType >
        class TDefaultStringHasher
        {
        public:
            typedef TCharType                               CharType;
            typedef Algorithm::TFormatPattern<CharType>     FormatPattern;
            typedef typename FormatPattern::SizeType        SizeType;

            /// <summary>
            /// Operator()s the specified start.
            /// </summary>
            /// <param name="start">The start.</param>
            /// <param name="length">The length.</param>
            /// <returns>SizeType.</returns>
            SizeType operator ()(const CharType* const start, SizeType length)
            {
                const unsigned char* const StartTest = (const unsigned char* const)start;

                SizeType Count = length*sizeof(CharType);

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

        /// <summary>
        /// Class TSTLConstIterator.
        /// </summary>
        template < typename TContainer >
        class TSTLConstIterator
        {
        public:
            typedef typename TContainer::const_iterator            ConstIterator;

            typedef typename ConstIterator::pointer                PointerType;
            typedef typename ConstIterator::reference              ReferenceType;

        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="TSTLConstIterator"/> class.
            /// </summary>
            /// <param name="con">The con.</param>
            TSTLConstIterator(const TContainer& con) :
                ContainerRef(con),
                CurrentIter(con.begin())
            {
            }

        public:
            /// <summary>
            /// Returns true if ... is valid.
            /// </summary>
            /// <returns>bool.</returns>
            inline bool       IsValid() const
            {
                return CurrentIter != ContainerRef.end();
            }

            /// <summary>
            /// Operator*s this instance.
            /// </summary>
            /// <returns>ReferenceType.</returns>
            ReferenceType operator*() const
            {
                return *CurrentIter;
            }

            /// <summary>
            /// Nexts this instance.
            /// </summary>
            /// <returns>bool.</returns>
            inline bool       Next()
            {
                assert(IsValid() && "Next Handle of Iterator Need Is Valid.");

                ++CurrentIter;

                return IsValid();
            }

        protected:
            /// <summary>
            /// The container reference
            /// </summary>
            const TContainer& ContainerRef;
            /// <summary>
            /// The current iter
            /// </summary>
            ConstIterator     CurrentIter;
        };

        /// <summary>
        /// Class TStandardPolicy.
        /// </summary>
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

            /// <summary>
            /// Finds the by hash key.
            /// </summary>
            /// <param name="storageReference">The storage reference.</param>
            /// <param name="hashKey">The hash key.</param>
            /// <returns>const PatternListType *.</returns>
            static const PatternListType* FindByHashKey(const PatternMapType& storageReference, SizeType hashKey)
            {
                typename PatternMapType::const_iterator itPos = storageReference.find(hashKey);

                return itPos != storageReference.end() ? &itPos->second : NULL;
            }

            /// <summary>
            /// Reserves the list.
            /// </summary>
            /// <param name="">The .</param>
            /// <param name="">The .</param>
            static void ReserveList(PatternListType& /*ListRef*/, int /*Len*/)
            {
                // AtuoArray does not need reserve
            }

            /// <summary>
            /// Emplaces the specified storage reference.
            /// </summary>
            /// <param name="storageReference">The storage reference.</param>
            /// <param name="hashKey">The hash key.</param>
            /// <param name="patterns">The patterns.</param>
            /// <returns>const PatternListType *.</returns>
            static const PatternListType* Emplace(
                PatternMapType& storageReference,
                SizeType hashKey,
#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
                PatternListType&& patterns
#else
                const PatternListType& patterns
#endif
                )
            {
                std::pair< typename PatternMapType::iterator, bool> Results =
#if FL_PLATFORM_HAS_RIGHT_VALUE_REFERENCE
                    storageReference.emplace(std::make_pair(hashKey, std::move(patterns)));
#else
                    storageReference.insert(std::make_pair(hashKey, patterns));
#endif

                return Results.second ? &Results.first->second : NULL;
            }

            /// <summary>
            /// Appends the pattern.
            /// </summary>
            /// <param name="patterns">The patterns.</param>
            /// <param name="pattern">The pattern.</param>
            static void AppendPattern(PatternListType& patterns, const FormatPattern& pattern)
            {
                patterns.AddItem(pattern);
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

