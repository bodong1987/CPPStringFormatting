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

#include <Format/Details/PatternParser.hpp>
#include <Format/Common/Mutex.hpp>

// ReSharper disable once CppEnforceNestedNamespacesStyle
namespace Formatting // NOLINT(*-concat-nested-namespaces)
{
    namespace Details
    {
        namespace Utils
        {
            template<typename TPolicy, bool bNeedLock>
            class TPatternStorageBase
            {
            public:
                typedef typename TPolicy::CharType                      CharType;
                typedef typename TPolicy::ByteType                      ByteType;
                typedef typename TPolicy::SizeType                      SizeType;
                typedef typename TPolicy::PatternListType               PatternListType;
                typedef typename TPolicy::PatternIterator               PatternIterator;
                typedef typename TPolicy::PatternMapType                PatternMapType;
                typedef typename TPolicy::ExceptionType                 ExceptionType;
                typedef typename TPolicy::MutexType                     MutexType;
                typedef TFormatPattern<CharType>                        FormatPattern;
                typedef TPatternParser<TPolicy>                         PatternParser;
                typedef TSharedLocker<MutexType>                        SharedLockerType;
                typedef TUniqueLocker<MutexType>                        UniqueLockerType;

            protected:
                /// <summary>
                /// Lookups the patterns with lockers
                /// </summary>
                /// <param name="formatStart">The format start.</param>
                /// <param name="length">The length.</param>
                /// <param name="hashKey">The hash key.</param>
                /// <returns>const PatternListType *.</returns>
                const PatternListType* LookupPatternsInternal(const CharType* const formatStart, const SizeType length, SizeType hashKey)
                {
                    // First, Find in the cache
                    {
                        SharedLockerType Locker(MutexValue);

                        const PatternListType* PatternList = TPolicy::FindByHashKey(Storage, hashKey);

                        if (nullptr != PatternList)
                        {
                            return PatternList;
                        }
                    }


                    PatternListType Patterns;
                    TPolicy::ReserveList(Patterns, 8);

                    PatternParser Parser;
                    if (Parser(formatStart, length, Patterns))
                    {
                        UniqueLockerType Locker(MutexValue);

                        return TPolicy::Emplace(Storage, hashKey, FL_MOVE_SEMANTIC(Patterns));
                    }

                    assert(false && "invalid format expression!");

                    // ReSharper disable once CppDFAUnreachableCode
                    throw ExceptionType("invalid format expression!");

                    // return nullptr;
                }

                MutexType                                               MutexValue;

                PatternMapType                                          Storage;
            };

            template<typename TPolicy>
            class TPatternStorageBase<TPolicy, false>
            {
            public:
                typedef typename TPolicy::CharType                      CharType;
                typedef typename TPolicy::ByteType                      ByteType;
                typedef typename TPolicy::SizeType                      SizeType;
                typedef typename TPolicy::PatternListType               PatternListType;
                typedef typename TPolicy::PatternIterator               PatternIterator;
                typedef typename TPolicy::PatternMapType                PatternMapType;
                typedef typename TPolicy::ExceptionType                 ExceptionType;
                typedef typename TPolicy::MutexType                     MutexType;
                typedef TFormatPattern<CharType>                        FormatPattern;
                typedef TPatternParser<TPolicy>                         PatternParser;
                typedef TSharedLocker<MutexType>                        SharedLockerType;
                typedef TUniqueLocker<MutexType>                        UniqueLockerType;

            protected:
                /// <summary>
                /// Lookups the patterns without lockers
                /// </summary>
                /// <param name="formatStart">The format start.</param>
                /// <param name="length">The length.</param>
                /// <param name="hashKey">The hash key.</param>
                /// <returns>const PatternListType *.</returns>
                const PatternListType* LookupPatternsInternal(const CharType* const formatStart, const SizeType length, SizeType hashKey)
                {
                    // First, Find in the cache
                    const PatternListType* PatternList = TPolicy::FindByHashKey(Storage, hashKey);

                    if (nullptr != PatternList)
                    {
                        return PatternList;
                    }

                    PatternListType Patterns;
                    TPolicy::ReserveList(Patterns, 8);

                    PatternParser Parser;
                    if (Parser(formatStart, length, Patterns))
                    {
                        return TPolicy::Emplace(Storage, hashKey, FL_MOVE_SEMANTIC(Patterns));
                    }

                    assert(false && "invalid format expression!");

                    // ReSharper disable once CppDFAUnreachableCode
                    throw ExceptionType("invalid format expression!");

                    // return nullptr;
                }

                PatternMapType                                              Storage;
            };
        }

        template < typename TPolicy >
        class TPatternStorage :
            public Utils::TPatternStorageBase<TPolicy, Mpl::IsSame<SharedMutexNone, typename TPolicy::MutexType>::Value>
        {
        public:
            typedef Utils::TPatternStorageBase<
                        TPolicy,
                        Mpl::IsSame<
                            SharedMutexNone,
                            typename TPolicy::MutexType
                        >::Value
                    >                                                       Super;

            typedef typename TPolicy::CharType                              CharType;
            typedef typename TPolicy::SizeType                              SizeType;
            typedef typename TPolicy::PatternListType                       PatternListType;
            typedef typename TPolicy::PatternMapType                        PatternMapType;

            /// <summary>
            /// Lookups the patterns without lockers
            /// </summary>
            /// <param name="formatStart">The format start.</param>
            /// <param name="length">The length.</param>
            /// <param name="hashKey">The hash key.</param>
            /// <returns>const PatternListType *.</returns>
            const PatternListType* LookupPatterns(const CharType* const formatStart, const SizeType length, SizeType hashKey)
            {
                return Super::LookupPatternsInternal(formatStart, length, hashKey);
            }
        };

        template < typename TPolicy >
        class TGlobalPatternStorage :
            public TPatternStorage<TPolicy>
        {
        public:
            /// <summary>
            /// Gets the storage.
            /// </summary>
            /// <returns>Formatting.Details.TGlobalPatternStorage&lt;TPolicy&gt; *.</returns>
            static TGlobalPatternStorage* GetStorage()
            {
#if FL_WITH_THREAD_LOCAL
                // ReSharper disable once CppRedundantStaticSpecifierOnThreadLocalLocalVariable
                static FL_THREAD_LOCAL TGlobalPatternStorage StaticStorage;
                return &StaticStorage;
#else
                #if !FL_WITH_MULTITHREAD_SUPPORT
                #error "normal static storage need disable multi thread support"
                #endif

                static TGlobalPatternStorage StaticStorage;
                return &StaticStorage;
#endif
            }
        };
    }
}

