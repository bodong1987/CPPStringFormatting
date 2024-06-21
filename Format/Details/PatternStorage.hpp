#pragma once

#include <Format/Details/PatternParser.hpp>
#include <Format/Common/Mutex.hpp>

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
            typedef typename TPolicy::MutexType                     MutexType;
            typedef TScopedLocker<MutexType>                        ScopedLockerType;
            typedef TFormatPattern<CharType>                        FormatPattern;
            typedef TPatternParser<TPolicy>                         PatternParser;

            TPatternStorage()
            {
#if FL_DEBUG
                //printf("created storage\n");
#endif
            }

            ~TPatternStorage()
            {
#if FL_DEBUG
                //printf("destroy storage\n");
#endif
            }
                        
            const PatternListType* LookupPatterns(const CharType* const formatStart, const SizeType length, SizeType hashKey = 0)
            {
                if (0 == hashKey)
                {
                    hashKey = Hasher(formatStart, length);
                }

                // First, Find in the cache
                {
                    ScopedLockerType Locker(MutexValue);

                    const PatternListType* PatternList = TPolicy::FindByHashKey(Storage, hashKey);

                    if (nullptr != PatternList)
                    {
                        return PatternList;
                    }
                }
                

                PatternListType Patterns;
                TPolicy::ReserveList(Patterns, 8);

                if (Parser(formatStart, length, Patterns))
                {
                    ScopedLockerType Locker(MutexValue);

                    return TPolicy::Emplace(Storage, hashKey, FL_MOVE_SEMANTIC(Patterns));
                }

                assert(false && "invalid format expression!");

                throw ExceptionType("invalid format expression!");

                return nullptr;
            }

        protected:
            /// <summary>
            /// The critical section value
            /// </summary>
            MutexType                   MutexValue;

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
#if FL_WITH_THREAD_LOCAL
                struct ManagedStorage
                {
                    typedef TScopedLocker<Mutex>                             LockerType;

                    Mutex                                                    MutexValue;
                    TAutoArray<TGlobalPatternStorage*>                       Storages;

                    ~ManagedStorage()
                    {
                        LockerType Locker(MutexValue);

                        for( SIZE_T i=0; i<Storages.GetLength(); ++i )
                        {
                            delete Storages[i];
                        }
                    }

                    void AddStorage( TGlobalPatternStorage* InputStorage )
                    {
                        assert(InputStorage);

                        LockerType Locker(MutexValue);

                        Storages.AddItem(InputStorage);
                    }
                };

                static ManagedStorage StaticManager;

                static FL_THREAD_LOCAL TGlobalPatternStorage* StaticStorage = NULL;

                if( !StaticStorage )
                {
                    StaticStorage = new TGlobalPatternStorage();

                    StaticManager.AddStorage(StaticStorage);
                }

                return StaticStorage;
#else
                static TGlobalPatternStorage StaticStorage;
                return &StaticStorage;
#endif
            }
        };
    }
}

