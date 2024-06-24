#include <Format/Common/Build.hpp>

#if FL_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable:4616)
#endif
#include <gtest/gtest.h>
#if FL_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

#include <Format/Format.hpp>
#include <Format/StandardLibraryAdapter.hpp>

using namespace Formatting;

class Vector3
{
public:
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    std::string ToString() const
    {
        char buffer[100];
        TCharTraits<char>::StringPrintf(buffer, _countof(buffer), "Vector3(%.2f, %.2f, %.2f)", x, y, z);
        return std::string(buffer);
    }

    std::wstring ToWString() const
    {
        wchar_t buffer[100];
        TCharTraits<wchar_t>::StringPrintf(buffer, _countof(buffer), L"Vector3(%.2f, %.2f, %.2f)", x, y, z);
        return std::wstring(buffer);
    }


private:
    float x, y, z;
};

/*
if you disable this translator
you will get a compile error of Transfer can't access :
vs2022:
\CPPFormatLibrary\Format\Details\FormatTo.hpp(83,68): error C2280: ��bool Formatting::Details::TTranslator<TCharType,Formatting::Details::Utils::DoTransferHelper<TCharType,TPatternType,T0>::DoTransfer::TransferType>::Transfer(Formatting::TAutoString<TCharType> &,const Formatting::Details::TFormatPattern<TCharType> &,const T &)��: Try to reference a deleted function
\CPPFormatLibrary\Format\Details\FormatTo.hpp(83,68): error C2280:         with
\CPPFormatLibrary\Format\Details\FormatTo.hpp(83,68): error C2280:         [
\CPPFormatLibrary\Format\Details\FormatTo.hpp(83,68): error C2280:             TCharType=wchar_t,
\CPPFormatLibrary\Format\Details\FormatTo.hpp(83,68): error C2280:             TPatternType=Formatting::Details::FormatTo::FormatPatternType,
\CPPFormatLibrary\Format\Details\FormatTo.hpp(83,68): error C2280:             T0=Vector3,
\CPPFormatLibrary\Format\Details\FormatTo.hpp(83,68): error C2280:             T=Formatting::Details::Utils::DoTransferHelper<wchar_t,Formatting::Details::FormatTo::FormatPatternType,Vector3>::DoTransfer::TransferType
\CPPFormatLibrary\Format\Details\FormatTo.hpp(83,68): error C2280:         ]

xcode:
 Showing All Messages
 /Users/bodong/Desktop/CPPFormatLibrary/UnitTests/Sources/UnitTests_Extends.cpp:12:10: in file included from /Users/bodong/Desktop/CPPFormatLibrary/UnitTests/Sources/UnitTests_Extends.cpp:12:
 /Users/bodong/Desktop/CPPFormatLibrary/Format/Format.hpp:39:10: in file included from /Users/bodong/Desktop/CPPFormatLibrary/Format/Format.hpp:39:
 /Users/bodong/Desktop/CPPFormatLibrary/Format/Details/FormatTo.hpp:98:77: In instantiation of member function 'Formatting::Details::Utils::DoTransferHelper<char, Formatting::Details::TFormatPattern<char>, Vector3>::DoTransfer' requested here
 /Users/bodong/Desktop/CPPFormatLibrary/Format/Details/FormatTo.hpp:154:33: In instantiation of function template specialization 'Formatting::Details::Utils::DoTransfer<char, Formatting::Details::TFormatPattern<char>, Vector3>' requested here
 /Users/bodong/Desktop/CPPFormatLibrary/Format/Details/StandardLibrary/FormatTo.hpp:88:22: In instantiation of function template specialization 'Formatting::Details::FormatTo<char, Formatting::Details::TGlobalPatternStorage<Formatting::Details::StandardLibrary::TStandardPolicy<char, Formatting::Details::MutexNone>>, const char *, Vector3>' requested here
 /Users/bodong/Desktop/CPPFormatLibrary/UnitTests/Sources/UnitTests_Extends.cpp:118:32: In instantiation of function template specialization 'Formatting::StandardLibrary::Format<char, Vector3>' requested here
 /Users/bodong/Desktop/CPPFormatLibrary/Format/Details/Translators.hpp:138:25: 'Transfer' has been explicitly marked deleted here

 gcc:
 In file included from I:/CPPFormatLibrary/Format/Format.hpp:39,
                 from I:\CPPFormatLibrary\UnitTests\Sources\UnitTests_Extends.cpp:12:
I:/CPPFormatLibrary/Format/Details/FormatTo.hpp: In instantiation of 'static bool Formatting::Details::Utils::DoTransferHelper<TCharType, TPatternType, T0>::DoTransfer(int32_t, int32_t&, Formatting::TAutoString<TCharType>&, const TPatternType&, const TCharType*, const T0&) [with TCharType = char; TPatternType = Formatting::Details::TFormatPattern<char>; T0 = Vector3; int32_t = int]':
I:/CPPFormatLibrary/Format/Details/FormatTo.hpp:98:87:   required from 'bool Formatting::Details::Utils::DoTransfer(int32_t, int32_t&, Formatting::TAutoString<TCharType>&, const TPatternType&, const TCharType*, const T0&, const T& ...) [with TCharType = char; TPatternType = Formatting::Details::TFormatPattern<char>; T0 = Vector3; T = {}; int32_t = int]'
I:/CPPFormatLibrary/Format/Details/FormatTo.hpp:154:79:   required from 'Formatting::TAutoString<TCharType>& Formatting::Details::FormatTo(Formatting::TAutoString<TCharType>&, const TFormatType&, const T& ...) [with TCharType = char; TPatternStorageType = Formatting::Details::TGlobalPatternStorage<Formatting::Details::StandardLibrary::TStandardPolicy<char, Formatting::Details::Mutex> >; TFormatType = const char*; T = {Vector3}]'
I:/CPPFormatLibrary/Format/Details/StandardLibrary/FormatTo.hpp:88:95:   required from 'std::__cxx11::basic_string<_CharT> Formatting::StandardLibrary::Format(const TCharType*, const T0&, T ...) [with TCharType = char; T0 = Vector3; T = {}]'
I:\CPPFormatLibrary\UnitTests\Sources\UnitTests_Extends.cpp:124:5:   required from here
I:/CPPFormatLibrary/Format/Details/FormatTo.hpp:83:76: error: use of deleted function 'static bool Formatting::Details::TTranslator<TCharType, T>::Transfer(typename Formatting::Details::TTranslator<TCharType, T>::Super::StringType&, const typename Formatting::Details::TTranslator<TCharType, T>::Super::FormatPattern&, const T&) [with TCharType = char; T = Vector3; typename Formatting::Details::TTranslator<TCharType, T>::Super::StringType = Formatting::TAutoString<char>; typename Formatting::Details::TTranslator<TCharType, T>::Super::FormatPattern = Formatting::Details::TFormatPattern<char>]'
                         if (!TTranslator<TCharType, TransferType>::Transfer(sink, pattern, arg0))
                              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~
In file included from I:/CPPFormatLibrary/Format/Details/FormatTo.hpp:28,
                 from I:/CPPFormatLibrary/Format/Format.hpp:39,
                 from I:\CPPFormatLibrary\UnitTests\Sources\UnitTests_Extends.cpp:12:
I:/CPPFormatLibrary/Format/Details/Translators.hpp:138:25: note: declared here
             static bool Transfer(typename Super::StringType& strRef, const typename Super::FormatPattern& pattern, const T& arg) = delete;
                         ^~~~~~~~

*/
#if 0
// The following class provides automatic conversion capabilities for Vector3, so that Vector3 type parameters can be formatted directly.
namespace Formatting
{
    namespace Details
    {
        // convert Vector3 to string
        template <>
        class TTranslator< char, Vector3 > :
            public TTranslatorBase< char, Vector3 >
        {
        public:
            typedef TTranslatorBase< char, Vector3 >           Super;
            typedef Super::CharType                            CharType;
            typedef Super::FormatPattern                       FormatPattern;
            typedef Super::ByteType                            ByteType;
            typedef Super::SizeType                            SizeType;
            typedef Super::StringType                          StringType;
            typedef Super::CharTraits                          CharTraits;

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, const Vector3& arg)
            {
                std::string text = arg.ToString();

                Super::AppendString(strRef, pattern, text.c_str(), text.size());

                return true;
            }
        };

        // convert Vector3 to wstring
        template <>
        class TTranslator< wchar_t, Vector3 > :
            public TTranslatorBase< wchar_t, Vector3 >
        {
        public:
            typedef TTranslatorBase< wchar_t, Vector3 >        Super;
            typedef Super::CharType                            CharType;
            typedef Super::FormatPattern                       FormatPattern;
            typedef Super::ByteType                            ByteType;
            typedef Super::SizeType                            SizeType;
            typedef Super::StringType                          StringType;
            typedef Super::CharTraits                          CharTraits;

            static bool Transfer(StringType& strRef, const FormatPattern& pattern, const Vector3& arg)
            {
                std::wstring text = arg.ToWString();

                Super::AppendString(strRef, pattern, text.c_str(), text.size());

                return true;
            }
        };
    }
}
#endif

TEST(FormatExtends, Extend_Vector3)
{
    Vector3 v(1,2,3);
    EXPECT_EQ(StandardLibrary::Format("{0}", v), "Vector3(1.00, 2.00, 3.00)");
    EXPECT_EQ(StandardLibrary::Format(L"{0}", v), L"Vector3(1.00, 2.00, 3.00)");
}
