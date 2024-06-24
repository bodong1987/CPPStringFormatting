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

#include <Format/Details/PatternStorage.hpp>

#if FL_COMPILER_WITH_CXX11
#include <unordered_map>
#else
#include <map>
#endif

namespace Formatting
{
    namespace Details
    {
        namespace StandardLibrary
        {
            /// <summary>
            /// Class TDefaultStringHasher.
            /// stl default string hasher
            /// </summary>
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
                    {
                        // fold in another byte
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
            /// Class TStandardPolicy.
            /// stl default policy
            /// </summary>
            template <typename TCharType, typename TMutexType>
            class TStandardPolicy
            {
            public:
                typedef TCharType                                              CharType;
                typedef TFormatPattern<CharType>                               FormatPattern;
                typedef typename FormatPattern::SizeType                       SizeType;
                typedef typename FormatPattern::ByteType                       ByteType;
                typedef TAutoArray<FormatPattern, 0xF, 0>                      PatternListType;
                typedef typename PatternListType::ConstIterator                PatternIterator;                
                typedef TDefaultStringHasher<CharType>                         HasherType;
                typedef std::runtime_error                                     ExceptionType;
                typedef TMutexType                                             MutexType;

#if FL_COMPILER_WITH_CXX11
                typedef std::unordered_map<SizeType, PatternListType>          PatternMapType;
#else
                typedef std::map<SizeType, PatternListType>                    PatternMapType;
#endif

                static const PatternListType* FindByHashKey(const PatternMapType& storageReference, SizeType hashKey)
                {
                    typename PatternMapType::const_iterator itPos = storageReference.find(hashKey);

                    return itPos != storageReference.end() ? &itPos->second : nullptr;
                }

                static void ReserveList(PatternListType& /*ListRef*/, int /*Len*/)
                {
                    // AutoArray does not need reserve
                }

                static const PatternListType* Emplace(
                    PatternMapType& storageReference, 
                    SizeType hashKey, 
#if FL_COMPILER_HAS_RIGHT_VALUE_REFERENCE
                    PatternListType&& patterns
#else
                    const PatternListType& patterns
#endif
                    )
                {
                    std::pair< typename PatternMapType::iterator, bool> Results = 
#if FL_COMPILER_HAS_RIGHT_VALUE_REFERENCE
                        storageReference.emplace(std::make_pair(hashKey, std::move(patterns)));
#else
                        storageReference.insert(std::make_pair(hashKey, patterns));
#endif

                    return Results.second ? &Results.first->second : nullptr;
                }

                static void AppendPattern(PatternListType& patterns, const FormatPattern& pattern)
                {
                    patterns.AddItem(pattern);
                }
            };

#if FL_WITH_THREAD_LOCAL || !FL_WITH_MULTITHREAD_SUPPORT
            typedef MutexNone                                       DefaultMutexType;
#else
            typedef Mutex                                           DefaultMutexType;
#endif

            typedef TPatternStorage< TStandardPolicy<char, DefaultMutexType> >          STLPatternStorageA;
            typedef TPatternStorage< TStandardPolicy<wchar_t, DefaultMutexType> >       STLPatternStorageW;
            typedef TGlobalPatternStorage< TStandardPolicy<char, DefaultMutexType> >    STLGlobalPatternStorageA;
            typedef TGlobalPatternStorage< TStandardPolicy<wchar_t, DefaultMutexType> > STLGlobalPatternStorageW;
        }        
    }
}
