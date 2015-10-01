// FormatLibrary 
// @author bodong 
// Parse and Manage Patterns
#pragma once

namespace FormatLibrary
{
    namespace Algorithm
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
            typedef typename TPolicy::CriticalSectionType           CriticalSectionType;
            typedef typename TPolicy::HasherType                    HasherType;
            typedef typename TPolicy::ExceptionType                 ExceptionType;
            typedef TFormatPattern<CharType>                        FormatPattern;
            typedef Utility::TScopedLocker<CriticalSectionType>     ScopedLockerType;      
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

            const PatternListType* LookupPatterns(const CharType* const FormatStart, const SizeType Length, SizeType HashKey = 0)
            {
                if (0 == HashKey)
                {
                    HashKey = Hasher(FormatStart, Length);
                }

                // First, Find in the cache
                {
                    ScopedLockerType Locker(CriticalSectionValue);

                    const PatternListType* PatternList = TPolicy::FindByHashKey(Storage, HashKey);

                    if (NULL != PatternList)
                    {
                        return PatternList;
                    }
                }

                PatternListType Patterns;
                TPolicy::ReserveList(Patterns, 8);

                if (Parser(FormatStart, Length, Patterns))
                {
                    ScopedLockerType Locker(CriticalSectionValue);

                    return TPolicy::Emplace(Storage, HashKey, FL_MOVE_SEMANTIC(Patterns));
                }

                assert(false && "invalid format expression!");

                throw ExceptionType("invalid format expression!");

                return NULL;
            }

        protected:
            HasherType                  Hasher;
            CriticalSectionType         CriticalSectionValue;
            PatternMapType              Storage;
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
                    typedef Utility::TScopedLocker<System::CriticalSection>  LockerType;
                    
                    System::CriticalSection                                  ManagedCS;
                    Utility::TAutoArray<TGlobalPatternStorage*>              Storages;
                    
                    ~ManagedStorage()
                    {
                        LockerType Locker(ManagedCS);
                        
                        for( SIZE_T i=0; i<Storages.GetLength(); ++i )
                        {
                            delete Storages[i];
                        }
                    }
                    
                    void AddStorage( TGlobalPatternStorage* InputStorage )
                    {
                        assert(InputStorage);
                        
                        LockerType Locker(ManagedCS);
                        
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