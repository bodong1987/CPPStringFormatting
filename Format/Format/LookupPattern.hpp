// FormatLibrary 
// @author bodong 
// Parse and Manage Patterns
#pragma once

/// <summary>
/// The FormatLibrary namespace.
/// </summary>
namespace FormatLibrary
{
    /// <summary>
    /// The Algorithm namespace.
    /// </summary>
    namespace Algorithm
    {
        /// <summary>
        /// Class TPatternStorage.
        /// Simple PatternStorage, You can provide your custom storage.
        /// </summary>
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
            
            /// <summary>
            /// Initializes a new instance of the <see cref="TPatternStorage"/> class.
            /// </summary>
            TPatternStorage()
            {
#if FL_DEBUG
                //printf("created storage\n");
#endif
            }
            
            /// <summary>
            /// Finalizes an instance of the <see cref="TPatternStorage"/> class.
            /// </summary>
            ~TPatternStorage()
            {
#if FL_DEBUG
                //printf("destroy storage\n");
#endif
            }

            /// <summary>
            /// Lookups the patterns.
            /// </summary>
            /// <param name="formatStart">The format start.</param>
            /// <param name="length">The length.</param>
            /// <param name="hashKey">The hash key.</param>
            /// <returns>const PatternListType *.</returns>
            const PatternListType* LookupPatterns(const CharType* const formatStart, const SizeType length, SizeType hashKey = 0)
            {
                if (0 == hashKey)
                {
                    hashKey = Hasher(formatStart, length);
                }

                // First, Find in the cache
                {
                    ScopedLockerType Locker(CriticalSectionValue);

                    const PatternListType* PatternList = TPolicy::FindByHashKey(Storage, hashKey);

                    if (NULL != PatternList)
                    {
                        return PatternList;
                    }
                }

                PatternListType Patterns;
                TPolicy::ReserveList(Patterns, 8);

                if (Parser(formatStart, length, Patterns))
                {
                    ScopedLockerType Locker(CriticalSectionValue);

                    return TPolicy::Emplace(Storage, hashKey, FL_MOVE_SEMANTIC(Patterns));
                }

                assert(false && "invalid format expression!");

                throw ExceptionType("invalid format expression!");

                return NULL;
            }

        protected:
            /// <summary>
            /// The hasher
            /// </summary>
            HasherType                  Hasher;
            /// <summary>
            /// The critical section value
            /// </summary>
            CriticalSectionType         CriticalSectionValue;
            /// <summary>
            /// The storage
            /// </summary>
            PatternMapType              Storage;
            /// <summary>
            /// The parser
            /// </summary>
            PatternParser               Parser;
        };

        /// <summary>
        /// Class TGlobalPatternStorage.
        /// Default global storage
        /// Implements the <see cref="TPatternStorage{TPolicy}" />
        /// </summary>
        /// <seealso cref="TPatternStorage{TPolicy}" />
        template < typename TPolicy >
        class TGlobalPatternStorage : 
            public TPatternStorage<TPolicy>
        {
        public:
            /// <summary>
            /// Gets the storage.
            /// </summary>
            /// <returns>FormatLibrary.Algorithm.TGlobalPatternStorage&lt;TPolicy&gt; *.</returns>
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