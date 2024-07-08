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

TEST(Format, STL_Char_Format)
{
    const std::string i0 = "Hello CppMiniToolkit";
    std::string r0 = StandardLibrary::Format(i0.c_str());
    EXPECT_EQ(r0, i0);

    const std::string i1 = "Hello CppMiniToolkit {0}";
    std::string r1 = StandardLibrary::Format(i1.c_str(), 1024);
    EXPECT_EQ(r1, "Hello CppMiniToolkit 1024");

    const std::string r2 = StandardLibrary::Format("{0}--#--{1,8}--#--{2}", 100, -40.2f, " String ");
    EXPECT_EQ(r2, "100--#--  -40.20--#-- String ");

    const std::string r3 = StandardLibrary::Format("{0}--#--{1,8}--#--{1}", 100, -40.2f);
    EXPECT_EQ(r3, "100--#--  -40.20--#---40.20");

    const std::string r4 = StandardLibrary::Format("{0}--#--{1,8}--#--{3}", 100, -40.2f, std::string("xxx"));
    EXPECT_EQ(r4, "100--#--  -40.20--#--{3}");

    const std::string r5 = StandardLibrary::Format("{0}", char('c'), short(2));
    EXPECT_EQ(r5, "c");

    const std::string r6 = StandardLibrary::Format("0x{0:x}", 100, (unsigned long)(100));
    EXPECT_EQ(r6, "0x64");

    // gen compile error
    //StandardLibrary::Format("{0}", std::wstring(L"x"));
}

TEST(Format, STL_Char_FormatTo)
{
    std::string v;

    const std::string i0 = "Hello CppMiniToolkit";
    StandardLibrary::FormatTo(v, i0.c_str());
    EXPECT_EQ(v, i0);

    const std::string i1 = "Hello CppMiniToolkit {0}";
    StandardLibrary::FormatTo(v, i1.c_str(), 1024);
    EXPECT_EQ(v, "Hello CppMiniToolkit 1024");

    StandardLibrary::FormatTo(v, "{0}--#--{1,8}--#--{2}", 100, -40.2f, " String ");
    EXPECT_EQ(v, "100--#--  -40.20--#-- String ");

    StandardLibrary::FormatTo(v, "{0}--#--{1,8}--#--{1}", 100, -40.2f);
    EXPECT_EQ(v, "100--#--  -40.20--#---40.20");

    StandardLibrary::FormatTo(v, "{0}--#--{1,8}--#--{3}", 100, -40.2f, std::string("xxx"));
    EXPECT_EQ(v, "100--#--  -40.20--#--{3}");

    StandardLibrary::FormatTo(v, "{0}", char('c'), short(2));
    EXPECT_EQ(v, "c");

    StandardLibrary::FormatTo(v, "0x{0:x}", 100, (unsigned long)(100));
    EXPECT_EQ(v, "0x64");
}

TEST(Format, STL_WChar_Format)
{
    const std::wstring r7 = StandardLibrary::Format(L"Test{1}, {2:f4}, {0}, {0,4}", L" X ", 20, -10.005f);
    EXPECT_EQ(r7, L"Test20, -10.0050,  X ,   X ");

    const std::wstring r8 = StandardLibrary::Format(L"Test{1}, {2:f4}, {0}, {0,4}");
    EXPECT_EQ(r8, L"Test{1}, {2:f4}, {0}, {0,4}");

    const std::wstring r9 = StandardLibrary::Format(std::wstring(L"Test{1}, {2:f4}, {0}, {0,4}"), L" X ", 20, -10.005f);
    EXPECT_EQ(r9, L"Test20, -10.0050,  X ,   X ");

    const std::wstring r11 = StandardLibrary::Format(L"\u4F60\u597D : {0}", L"\u4E2D\u6587");
    EXPECT_EQ(r11, L"\u4F60\u597D : \u4E2D\u6587");
}

TEST(Format, STL_WChar_FormatTo)
{
    std::wstring v;

    StandardLibrary::FormatTo(v, L"Test{1}, {2:f4}, {0}, {0,4}", L" X ", 20, -10.005f);
    EXPECT_EQ(v, L"Test20, -10.0050,  X ,   X ");

    // test invalid param
    StandardLibrary::FormatTo(v, L"Test{1}, {2:f4}, {0}, {0,4}");
    EXPECT_EQ(v, L"Test{1}, {2:f4}, {0}, {0,4}");

    StandardLibrary::FormatTo(v, std::wstring(L"Test{1}, {2:f4}, {0}, {0,4}"), L" X ", 20, -10.005f);
    EXPECT_EQ(v, L"Test20, -10.0050,  X ,   X ");

    StandardLibrary::FormatTo(v, L"\u4F60\u597D : {0}", L"\u4E2D\u6587");
    EXPECT_EQ(v, L"\u4F60\u597D : \u4E2D\u6587");
}

TEST(Format, TestSingleArg)
{
    EXPECT_EQ(StandardLibrary::Format("{0}", 123), "123");
}

TEST(Format, TestMultipleArgs)
{
    EXPECT_EQ(StandardLibrary::Format("{0} {1}", 123, "hello"), "123 hello");
}

TEST(Format, TestReorderedArgs)
{
    EXPECT_EQ(StandardLibrary::Format("{1} {0}", 123, "hello"), "hello 123");
}

TEST(Format, TestRepeatedArgs)
{
    EXPECT_EQ(StandardLibrary::Format("{0} {0}", 123), "123 123");
}

TEST(Format, TestString)
{
    EXPECT_EQ(StandardLibrary::Format("{0}", "hello"), "hello");
}

TEST(Format, TestChar)
{
    EXPECT_EQ(StandardLibrary::Format("{0}", 'a'), "a");
}

TEST(Format, TestBool)
{
    EXPECT_EQ(StandardLibrary::Format("{0}", true), "True");
    EXPECT_EQ(StandardLibrary::Format("{0}", false), "False");
}

TEST(Format, TestEmptyFormat)
{
    EXPECT_EQ(StandardLibrary::Format("", 123), "");
}

TEST(Format, TestNoArgs)
{
    EXPECT_EQ(StandardLibrary::Format("hello"), "hello");
}

TEST(Format, TestMultipleFormats)
{
    EXPECT_EQ(StandardLibrary::Format("{0} {1} {2}", 123, "hello", 1.23), "123 hello 1.23");
}

TEST(Format, TestRightAlign)
{
    EXPECT_EQ(StandardLibrary::Format("{0,5}", 123), "  123");
}

TEST(Format, TestLeftAlign)
{
    EXPECT_EQ(StandardLibrary::Format("{0,-5}", 123), "123  ");
}

TEST(Format, TestRightAlignString)
{
    EXPECT_EQ(StandardLibrary::Format("{0,5}", "hi"), "   hi");
}

TEST(Format, TestLeftAlignString)
{
    EXPECT_EQ(StandardLibrary::Format("{0,-5}", "hi"), "hi   ");
}

TEST(Format, TestRightAlignMultiple)
{
    EXPECT_EQ(StandardLibrary::Format("{0,5} {1,5}", 123, "hi"), "  123    hi");
}

TEST(Format, TestLeftAlignMultiple)
{
    EXPECT_EQ(StandardLibrary::Format("{0,-5} {1,-5}", 123, "hi"), "123   hi   ");
}

TEST(Format, TestMixedAlign)
{
    EXPECT_EQ(StandardLibrary::Format("{0,5} {1,-5}", 123, "hi"), "  123 hi   ");
}

TEST(Format, TestPositiveNumberPadding)
{
    EXPECT_EQ(StandardLibrary::Format("{0,5}", 123), "  123");
    EXPECT_EQ(StandardLibrary::Format("{0,-5}", 123), "123  ");
}

TEST(Format, TestNegativeNumberPadding)
{
    EXPECT_EQ(StandardLibrary::Format("{0,6}", -123), "  -123");
    EXPECT_EQ(StandardLibrary::Format("{0,-6}", -123), "-123  ");
}

TEST(Format, TestEscapeSequence)
{
    EXPECT_EQ(StandardLibrary::Format("{0}", "\n\t\""), "\n\t\"");
}

TEST(Format, TestExponentialNotation)
{
    EXPECT_EQ(StandardLibrary::Format("{0:e}", 123.456), "1.234560e+002");
    EXPECT_EQ(StandardLibrary::Format("{0:E}", 123.456), "1.234560E+002");
    EXPECT_EQ(StandardLibrary::Format("{0:e3}", 123.456), "1.235e+002");
    EXPECT_EQ(StandardLibrary::Format("{0:E3}", 123.456), "1.235E+002");
}

TEST(Format, TestHexadecimalNotation)
{
    EXPECT_EQ(StandardLibrary::Format("{0:x}", 123), "7b");
    EXPECT_EQ(StandardLibrary::Format("{0:X}", 123), "7B");
    EXPECT_EQ(StandardLibrary::Format("{0:x8}", 123), "0000007b");
    EXPECT_EQ(StandardLibrary::Format("{0:X8}", 123), "0000007B");
}

TEST(Format, TestWidth)
{
    EXPECT_EQ(StandardLibrary::Format("{0,5}", 123), "  123");
    EXPECT_EQ(StandardLibrary::Format("{0,-5}", 123), "123  ");
    EXPECT_EQ(StandardLibrary::Format("{0,5}", "hi"), "   hi");
    EXPECT_EQ(StandardLibrary::Format("{0,-5}", "hi"), "hi   ");
}

TEST(Format, TestFloatingPointPrecision)
{
    EXPECT_EQ(StandardLibrary::Format("{0:f}", 123.456), "123.46");
    EXPECT_EQ(StandardLibrary::Format("{0:f3}", 123.456), "123.456");
    EXPECT_EQ(StandardLibrary::Format("{0:f6}", 123.456), "123.456000");
    EXPECT_EQ(StandardLibrary::Format("{0:f0}", 123.456), "123");
}

TEST(Format, TestFloatingPointPrecisionComplex)
{
    EXPECT_EQ(StandardLibrary::Format("{0:f4}", 123.456789), "123.4568");
    EXPECT_EQ(StandardLibrary::Format("{0:f2}", 123.456789), "123.46");
    EXPECT_EQ(StandardLibrary::Format("{0:f0}", 123.456789), "123");
    EXPECT_EQ(StandardLibrary::Format("{0:f1}", -123.456789), "-123.5");
    EXPECT_EQ(StandardLibrary::Format("{0:f6}", 0.000000123456789), "0.000000");
    EXPECT_EQ(StandardLibrary::Format("{0:f6}", 0.00000123456789), "0.000001");
    EXPECT_EQ(StandardLibrary::Format("{0:e}", 123456789.0), "1.234568e+008");
    EXPECT_EQ(StandardLibrary::Format("{0:e3}", 123456789.0), "1.235e+008");
    EXPECT_EQ(StandardLibrary::Format("{0:e}", 0.000000123456789), "1.234568e-007");
    EXPECT_EQ(StandardLibrary::Format("{0:e3}", 0.000000123456789), "1.235e-007");
}

TEST(Format, TestEscapeBraces)
{
    EXPECT_EQ(StandardLibrary::Format("{{0}}"), "{0}");
    EXPECT_EQ(StandardLibrary::Format("{{0}}1"), "{0}1");
    EXPECT_EQ(StandardLibrary::Format("a{{0}}1a"), "a{0}1a");
    EXPECT_EQ(StandardLibrary::Format("{{{0}}}", 1), "{1}");
    EXPECT_EQ(StandardLibrary::Format("{{0}}, {0}", 1), "{0}, 1");
}

TEST(Format, TestEscapeBracesComplex)
{
    EXPECT_EQ(StandardLibrary::Format("{{{{0}}}}"), "{{0}}");
    EXPECT_EQ(StandardLibrary::Format("{{{0}}}", 123), "{123}");
    EXPECT_EQ(StandardLibrary::Format("{{{{{0}}}}}", 123), "{{123}}");
    EXPECT_EQ(StandardLibrary::Format("{{{{0}}}}, {0}", 1), "{{0}}, 1");
    EXPECT_EQ(StandardLibrary::Format("{{{{0}}}}, {0}, {{{0}}}, {{{{0}}}}", 1), "{{0}}, 1, {1}, {{0}}");
    EXPECT_EQ(StandardLibrary::Format("{{0}} {0} {{0}} {0}", "Test"), "{0} Test {0} Test");
    EXPECT_EQ(StandardLibrary::Format("{{{{{0}}}}}", "Test"), "{{Test}}");
    EXPECT_EQ(StandardLibrary::Format("{{{{0}}}}, {0}, {{{0}}}, {{{{0}}}}", "Test"), "{{0}}, Test, {Test}, {{0}}");
}

TEST(Format, TestOtherCharacters)
{
    EXPECT_EQ(StandardLibrary::Format("{0} is {1} years old", "John", 30), "John is 30 years old");
}

TEST(Format, TestPointer)
{
    {
        const int* intPtr = new int[12];
        char szText[64] = {0};

#if FL_PLATFORM_MACOS
        TCharTraits<char>::StringPrintf(szText, FL_ARRAY_COUNTOF(szText), "0x%016lX", intPtr);
#else
        TCharTraits<char>::StringPrintf(szText, FL_ARRAY_COUNTOF(szText), "0x%p", intPtr);
#endif
    
        EXPECT_EQ(StandardLibrary::Format("0x{0:X}", intPtr), szText);
        EXPECT_EQ(StandardLibrary::Format("0x{0:X}", (void*)intPtr), szText);
        EXPECT_EQ(StandardLibrary::Format("0x{0:X}", (const void*)intPtr), szText);

        delete []intPtr;    
    }

    {
        int* intPtr = new int[12];
        wchar_t szText[64] = {0};

#if FL_PLATFORM_MACOS
        TCharTraits<wchar_t>::StringPrintf(szText, FL_ARRAY_COUNTOF(szText), L"0x%016lX", intPtr);
#else
        TCharTraits<wchar_t>::StringPrintf(szText, FL_ARRAY_COUNTOF(szText), L"0x%p", intPtr);
#endif
    
        EXPECT_EQ(StandardLibrary::Format(L"0x{0:X}", intPtr), szText);
        EXPECT_EQ(StandardLibrary::Format(L"0x{0:X}", (void*)intPtr), szText);
        EXPECT_EQ(StandardLibrary::Format(L"0x{0:X}", (const void*)intPtr), szText);

        delete []intPtr;
    }
}


TEST(Format, TestMultipleDifferentArgs)
{
    int a = 123;
    double b = 123.456;
    char c = 'a';
    const char* d = "hello";
    bool e = true;
    std::string f = "world";
    long g = 1234567890;
    unsigned int h = 123;
    float i = 1.23f;
    short j = 123;

    std::string result = StandardLibrary::Format("{0} {1:f3} {2} {3} {4} {5} {6} {7:x} {8:e} {9}",
        a, b, c, d, e, f, g, h, i, j);

    EXPECT_EQ(result, "123 123.456 a hello True world 1234567890 7b 1.230000e+000 123");
}

TEST(Format, TestLongTypes)
{
    unsigned long a = 1234567890;
    long long b = 123456789012345;
    unsigned long long c = 1234567890123456;

    std::string result = StandardLibrary::Format("{0} {1} {2}", a, b, c);
    EXPECT_EQ(result, "1234567890 123456789012345 1234567890123456");

    std::wstring resultW = StandardLibrary::Format(L"{0} {1} {2}", a, b, c);
    EXPECT_EQ(resultW, L"1234567890 123456789012345 1234567890123456");

    result = StandardLibrary::Format("{0:x} {1:x} {2:x}", a, b, c);
    EXPECT_EQ(result, "499602d2 7048860ddf79 462d53c8abac0");

    resultW = StandardLibrary::Format(L"{0:x} {1:x} {2:x}", a, b, c);
    EXPECT_EQ(resultW, L"499602d2 7048860ddf79 462d53c8abac0");
}

TEST(Format, TestMultipleDifferentArgsWChar)
{
    int a = 123;
    double b = 123.456;
    wchar_t c = L'a';
    const wchar_t* d = L"hello";
    bool e = true;
    std::wstring f = L"world";
    long g = 1234567890;
    unsigned int h = 123;
    float i = 1.23f;
    short j = 123;
    unsigned short k = 123;
    long double l = 123.456789;
    unsigned long m = 1234567890;
    long long n = 123456789012345;
    unsigned long long o = 1234567890123456;

    std::wstring result = StandardLibrary::Format(L"{0} {1:f3} {2} {3} {4} {5} {6} {7:x} {8:e} {9} {10} {11} {12} {13} {14}",
        a, b, c, d, e, f, g, h, i, j, k, l, m, n, o);

    EXPECT_EQ(result, L"123 123.456 a hello True world 1234567890 7b 1.230000e+000 123 123 123.46 1234567890 123456789012345 1234567890123456");
}

TEST(Format, TestLongString)
{
    std::string longStr(500, 'a');
    std::string longStr2(1000, 'b');
    std::string result = StandardLibrary::Format("{0} - {0} - NewString - {1}", longStr, longStr2);

    EXPECT_EQ(result, longStr + " - " + longStr + " - NewString - " + longStr2);
    EXPECT_GT(result.length(), 2000);
}

TEST(Format, TestBinaryStringNotation)
{
    EXPECT_EQ(StandardLibrary::Format("{0:B}", 123), "1111011");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", 123), "1111011");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", -123), "11111111111111111111111110000101");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", -123), "11111111111111111111111110000101");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", 0), "0");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", 0), "0");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", 1), "1");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", 1), "1");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", 255), "11111111");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", 255), "11111111");

    EXPECT_EQ(StandardLibrary::Format("{0:B8}", 123), "01111011");
    EXPECT_EQ(StandardLibrary::Format("{0:b8}", 123), "01111011");
    EXPECT_EQ(StandardLibrary::Format("{0:B8}", 0), "00000000");
    EXPECT_EQ(StandardLibrary::Format("{0:b8}", 0), "00000000");
    EXPECT_EQ(StandardLibrary::Format("{0:B8}", 1), "00000001");
    EXPECT_EQ(StandardLibrary::Format("{0:b8}", 1), "00000001");
    EXPECT_EQ(StandardLibrary::Format("{0:B8}", 255), "11111111");
    EXPECT_EQ(StandardLibrary::Format("{0:b8}", 255), "11111111");

    EXPECT_EQ(StandardLibrary::Format("{0:B}", 1024), "10000000000");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", 1024), "10000000000");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", 999999), "11110100001000111111");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", 999999), "11110100001000111111");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", 123456789), "111010110111100110100010101");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", 123456789), "111010110111100110100010101");

    EXPECT_EQ(StandardLibrary::Format("{0:B}", int64_t(9223372036854775807)), "111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", int64_t(9223372036854775807)), "111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", uint64_t(18446744073709551615ull)), "1111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", uint64_t(18446744073709551615ull)), "1111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", int64_t(1234567890123456789)), "1000100100010000100001111010001111101111010011000000100010101");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", int64_t(1234567890123456789)), "1000100100010000100001111010001111101111010011000000100010101");
    EXPECT_EQ(StandardLibrary::Format("{0:B}", uint64_t(9876543210987654321ull)), "1000100100010000100001111011100011100011101101110000110010110001");
    EXPECT_EQ(StandardLibrary::Format("{0:b}", uint64_t(9876543210987654321ull)), "1000100100010000100001111011100011100011101101110000110010110001");

    EXPECT_EQ(StandardLibrary::Format(L"{0:B}", int64_t(9223372036854775807)), L"111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(StandardLibrary::Format(L"{0:b}", int64_t(9223372036854775807)), L"111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(StandardLibrary::Format(L"{0:B}", uint64_t(18446744073709551615ull)), L"1111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(StandardLibrary::Format(L"{0:b}", uint64_t(18446744073709551615ull)), L"1111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(StandardLibrary::Format(L"{0:B}", int64_t(1234567890123456789)), L"1000100100010000100001111010001111101111010011000000100010101");
    EXPECT_EQ(StandardLibrary::Format(L"{0:b}", int64_t(1234567890123456789)), L"1000100100010000100001111010001111101111010011000000100010101");
    EXPECT_EQ(StandardLibrary::Format(L"{0:B}", uint64_t(9876543210987654321ull)), L"1000100100010000100001111011100011100011101101110000110010110001");
    EXPECT_EQ(StandardLibrary::Format(L"{0:b}", uint64_t(9876543210987654321ull)), L"1000100100010000100001111011100011100011101101110000110010110001");
}
