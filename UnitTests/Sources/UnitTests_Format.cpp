﻿// ReSharper disable CppCStyleCast
// ReSharper disable CppUseAuto
// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppFunctionalStyleCast
// ReSharper disable CppDFATimeOver
#include <Format/Common/Build.hpp>

#if FL_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable:4616)
#endif
#include <gtest/gtest.h>
#if FL_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

#include <iomanip>
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

#if FL_COMPILER_IS_GREATER_THAN_CXX17
    std::string_view v0 = "2048";
    std::string r7 = StandardLibrary::Format("Hello Formatting {0}", v0);
    EXPECT_EQ(r7, "Hello Formatting 2048");

    constexpr std::string_view sv = "Hello Formatting {0}";
    std::string r8 = StandardLibrary::Format(sv, 2048);
    EXPECT_EQ(r8, "Hello Formatting 2048");

    const std::string r8_1 = StandardLibrary::Format(sv);
    EXPECT_EQ(r8_1, sv);    
#endif

    const std::string r9 = StandardLibrary::Format(std::string("0x{0:x}"));
    EXPECT_EQ(r9, "0x{0:x}");
    
    // gen compile error
    //StandardLibrary::Format("{0}", std::wstring(L"x"));
}

#ifndef FL_DISABLE_STANDARD_LIBARY_MACROS
TEST(Format, STL_Char_Format_FL_STD_FORMAT)
{
    const std::string r2 = FL_STD_FORMAT("{0}--#--{1,8}--#--{2}", 100, -40.2f, " String ");
    EXPECT_EQ(r2, "100--#--  -40.20--#-- String ");
    
    const std::string r3 = FL_STD_FORMAT("{0}--#--{1,8}--#--{1}", 100, -40.2f);
    EXPECT_EQ(r3, "100--#--  -40.20--#---40.20");
    
    const std::string r4 = FL_STD_FORMAT("{0}--#--{1,8}--#--{3}", 100, -40.2f, std::string("xxx"));
    EXPECT_EQ(r4, "100--#--  -40.20--#--{3}");
    
    const std::string r5 = FL_STD_FORMAT("{0}", char('c'), short(2));
    EXPECT_EQ(r5, "c");
    
    const std::string r6 = FL_STD_FORMAT("0x{0:x}", 100, (unsigned long)(100));
    EXPECT_EQ(r6, "0x64");

    // can't compile
    // FL_CONSTEXPR20 const char* fmt = "0x{0:x}";
    // const std::string r7 = FL_STD_FORMAT(fmt, 100, (unsigned long)(100));
    // EXPECT_EQ(r6, "0x64");

    // can't compile 
    // const char* fmt = "0x{0:x}";
    // const std::string r7 = FL_STD_FORMAT(fmt, 100, (unsigned long)(100));
    // EXPECT_EQ(r6, "0x64");
}
#endif

#ifndef FL_DISABLE_STANDARD_LIBARY_MACROS
TEST(Format, STL_Char_FormatTo_FL_STD_FORMAT_TO)
{
    std::string v;

    // can't compile
    // const std::string i1 = "Hello CppMiniToolkit {0}";
    // FL_STD_FORMAT_TO(v, i1.c_str(), 1024);
    // EXPECT_EQ(v, "Hello CppMiniToolkit 1024");

    FL_STD_FORMAT_TO(v, "{0}--#--{1,8}--#--{2}", 100, -40.2f, " String ");
    EXPECT_EQ(v, "100--#--  -40.20--#-- String ");

    FL_STD_FORMAT_TO(v, "{0}--#--{1,8}--#--{1}", 100, -40.2f);
    EXPECT_EQ(v, "100--#--  -40.20--#---40.20");

    FL_STD_FORMAT_TO(v, "{0}--#--{1,8}--#--{3}", 100, -40.2f, std::string("xxx"));
    EXPECT_EQ(v, "100--#--  -40.20--#--{3}");

    FL_STD_FORMAT_TO(v, "{0}", char('c'), short(2));
    EXPECT_EQ(v, "c");

    FL_STD_FORMAT_TO(v, "0x{0:x}", 100, (unsigned long)(100));
    EXPECT_EQ(v, "0x64");
}
#endif


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

#if FL_COMPILER_IS_GREATER_THAN_CXX17
    constexpr std::wstring_view v0 = L"2048";
    const std::wstring r12 = StandardLibrary::Format(L"Hello Formatting {0}", v0);
    EXPECT_EQ(r12, L"Hello Formatting 2048");
#endif
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

#ifndef FL_DISABLE_STANDARD_LIBARY_MACROS
TEST(Format, STL_WChar_Format_FL_STD_FORMAT)
{
    const std::wstring r7 = FL_STD_FORMAT(L"Test{1}, {2:f4}, {0}, {0,4}", L" X ", 20, -10.005f);
    EXPECT_EQ(r7, L"Test20, -10.0050,  X ,   X ");

    const std::wstring r8 = FL_STD_FORMAT(L"Test{1}, {2:f4}, {0}, {0,4}");
    EXPECT_EQ(r8, L"Test{1}, {2:f4}, {0}, {0,4}");

    // can't compile
    // const std::wstring r9 = FL_STD_FORMAT(std::wstring(L"Test{1}, {2:f4}, {0}, {0,4}"), L" X ", 20, -10.005f);
    // EXPECT_EQ(r9, L"Test20, -10.0050,  X ,   X ");

    // can't compile
    // const std::wstring f10(L"Test{1}, {2:f4}, {0}, {0,4}");
    // const std::wstring r9 = FL_STD_FORMAT(f10.c_str(), L" X ", 20, -10.005f);
    // EXPECT_EQ(r9, L"Test20, -10.0050,  X ,   X ");

    const std::wstring r11 = FL_STD_FORMAT(L"\u4F60\u597D : {0}", L"\u4E2D\u6587");
    EXPECT_EQ(r11, L"\u4F60\u597D : \u4E2D\u6587");
}

TEST(Format, STL_WChar_FormatTo_FL_STD_FORMAT_TO)
{
    std::wstring v;

    FL_STD_FORMAT_TO(v, L"Test{1}, {2:f4}, {0}, {0,4}", L" X ", 20, -10.005f);
    EXPECT_EQ(v, L"Test20, -10.0050,  X ,   X ");

    // test invalid param
    FL_STD_FORMAT_TO(v, L"Test{1}, {2:f4}, {0}, {0,4}");
    EXPECT_EQ(v, L"Test{1}, {2:f4}, {0}, {0,4}");

    // gen compile error
    // FL_STD_FORMAT_TO(v, std::wstring(L"Test{1}, {2:f4}, {0}, {0,4}"), L" X ", 20, -10.005f);
    // EXPECT_EQ(v, L"Test20, -10.0050,  X ,   X ");

    FL_STD_FORMAT_TO(v, L"\u4F60\u597D : {0}", L"\u4E2D\u6587");
    EXPECT_EQ(v, L"\u4F60\u597D : \u4E2D\u6587");
}
#endif

#if FL_COMPILER_IS_GREATER_THAN_CXX11
TEST(Format, TestUTF_8)
{
    const std::string str = StandardLibrary::Format(
        u8"目标地址可能位于线程{0}(0x{0:X}){1}的栈内存空间内(0x{2:X}-0x{3:X})",
        100,
        u8"主线程",
        0,
        10000);

    EXPECT_EQ(str, u8"目标地址可能位于线程100(0x64)主线程的栈内存空间内(0x0-0x2710)");

    const std::string str2 = StandardLibrary::Format(
        u8"尝试对无效内存地址执行操作[{1}]. 错误地址: 0x{0:X}, 请结合异常上下文信息进行进一步分析.",
        static_cast<void*>(nullptr),
        u8"Write"
        );

    // ReSharper disable once CppIfCanBeReplacedByConstexprIf
    if (sizeof(void*) == 8)
    {
        EXPECT_EQ(str2, u8"尝试对无效内存地址执行操作[Write]. 错误地址: 0x0000000000000000, 请结合异常上下文信息进行进一步分析.");
    }
    else
    {
        EXPECT_EQ(str2, u8"尝试对无效内存地址执行操作[Write]. 错误地址: 0x00000000, 请结合异常上下文信息进行进一步分析.");
    }
}
#endif

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
        std::ostringstream oss;
        oss << "0x" << std::setw(16) << std::setfill('0') << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(intPtr);
        strcpy(szText, oss.str().c_str());

        EXPECT_STREQ(StandardLibrary::Format("0x{0:X}", intPtr).c_str(), szText);
        EXPECT_STREQ(StandardLibrary::Format("0x{0:X}", (void*)intPtr).c_str(), szText);
        EXPECT_STREQ(StandardLibrary::Format("0x{0:X}", (const void*)intPtr).c_str(), szText);

        delete []intPtr;    
    }

    {
        const int* intPtr = new int[12];
        wchar_t szText[64] = {0};

        std::wostringstream woss;
        woss << L"0x" << std::setw(16) << std::setfill(L'0') << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(intPtr);
        wcscpy(szText, woss.str().c_str());
    
        EXPECT_STREQ(StandardLibrary::Format(L"0x{0:X}", intPtr).c_str(), szText);
        EXPECT_STREQ(StandardLibrary::Format(L"0x{0:X}", (void*)intPtr).c_str(), szText);
        EXPECT_STREQ(StandardLibrary::Format(L"0x{0:X}", (const void*)intPtr).c_str(), szText);

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
