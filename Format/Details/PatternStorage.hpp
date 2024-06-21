#pragma once

#include <mutex>

#include <Format/Details/PatternParser.hpp>

namespace FormatLibrary
{
    namespace Details
    {
        template < typename TPolicy >
        class TPatternStorage
        {
        public:
            typedef typename TPolicy::CharType                      CharType;
            typedef typename TPolicy::ByteType                      ByteType;
            typedef typename TPolicy::SizeType                      SizeType;
            typedef typename TPolicy::PatternListType               PatternListType;
            typedef typename TPolicy::PatternIterator               PatternIterator;
            typedef typename TPolicy::PatternMapType                PatternMapType;
            typedef typename TPolicy::HasherType                    HasherType;
            typedef typename TPolicy::ExceptionType                 ExceptionType;
            typedef TFormatPattern<CharType>                        FormatPattern;
            typedef TPatternParser<TPolicy>                         PatternParser;

            TPatternStorage()
            {
            }

            ~TPatternStorage()
            {
            }
                        
            const PatternListType* LookupPatterns(const CharType* const formatStart, const SizeType length, SizeType hashKey = 0)
            {
                if (0 == hashKey)
                {
                    hashKey = Hasher(formatStart, length);
                }

                // First, Find in the cache
                const PatternListType* PatternList = TPolicy::FindByHashKey(Storage, hashKey);

                if (nullptr != PatternList)
                {
                    return PatternList;
                }

                PatternListType Patterns;
                TPolicy::ReserveList(Patterns, 8);

                if (Parser(formatStart, length, Patterns))
                {
                    return TPolicy::Emplace(Storage, hashKey, std::move(Patterns));
                }

                assert(false && "invalid format expression!");

                throw ExceptionType("invalid format expression!");

                return nullptr;
            }

        protected:
            /// <summary>
            /// The hasher
            /// </summary>
            HasherType                  Hasher;
            
            /// <summary>
            /// The storage
            /// </summary>
            PatternMapType              Storage;
            /// <summary>
            /// The parser
            /// </summary>
            PatternParser               Parser;
        };

        template < typename TPolicy >
        class TGlobalPatternStorage :
            public TPatternStorage<TPolicy>
        {
        public:
            static TGlobalPatternStorage* GetStorage()
            {
                struct ManagedStorage
                {
                    std::mutex _localMutext;
                    TAutoArray<std::shared_ptr<TGlobalPatternStorage>> _Storages;
                    
                    void AddStorage(std::shared_ptr<TGlobalPatternStorage>& InputStorage)
                    {
                        assert(InputStorage);

                        std::unique_lock<std::mutex> lock(_localMutext);

                        _Storages.AddItem(InputStorage);
                    }
                };

                static ManagedStorage StaticManager;

                static thread_local std::shared_ptr<TGlobalPatternStorage> StaticStorage;

                if (!StaticStorage)
                {
                    StaticStorage = std::make_shared<TGlobalPatternStorage>();

                    StaticManager.AddStorage(StaticStorage);
                }

                return StaticStorage.get();
            }
        };
    }
}

