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
#pragma once

#include <Format/Details/PatternStorage.hpp>
#include <stdexcept>

#if FL_COMPILER_IS_GREATER_THAN_CXX11
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
                typedef TAutoArray<FormatPattern, 0xF, 0>                      PatternListType; // NOLINT
                typedef typename PatternListType::ConstIterator                PatternIterator;
                typedef std::runtime_error                                     ExceptionType;
                typedef TMutexType                                             MutexType;

#if FL_COMPILER_IS_GREATER_THAN_CXX11
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
#if FL_COMPILER_IS_GREATER_THAN_CXX11
                    PatternListType&& patterns
#else
                    const PatternListType& patterns
#endif
                    )
                {
                    std::pair< typename PatternMapType::iterator, bool> Results = 
#if FL_COMPILER_IS_GREATER_THAN_CXX11
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
            typedef MutexNone                                                           DefaultMutexType;
#else
            typedef Mutex                                                               DefaultMutexType;
#endif

            typedef TPatternStorage< TStandardPolicy<char, DefaultMutexType> >          STLPatternStorageA;         // NOLINT
            typedef TPatternStorage< TStandardPolicy<wchar_t, DefaultMutexType> >       STLPatternStorageW;         // NOLINT
            typedef TGlobalPatternStorage< TStandardPolicy<char, DefaultMutexType> >    STLGlobalPatternStorageA;   // NOLINT
            typedef TGlobalPatternStorage< TStandardPolicy<wchar_t, DefaultMutexType> > STLGlobalPatternStorageW;   // NOLINT
        }        
    }
}
