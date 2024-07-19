// ReSharper disable CppUseStdSize
#include <Format/Common/Build.hpp>

#if FL_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable:4616)
#endif
#include <gtest/gtest.h>
#if FL_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

#include <Format/Common/Algorithm.hpp>
#include <Format/Details/StringConvertAlgorithm.hpp>

using namespace Formatting;

TEST(Algorithm, TestMoveArray)
{
    int src[] = {1, 2, 3, 4, 5};
    int dest[5] = {0};
    Algorithm::MoveArray(src, src + 5, dest);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(dest[i], i + 1);
    }
}

TEST(Algorithm, TestCopyArray)
{
    int src[] = {1, 2, 3, 4, 5};
    int dest[5] = {0};
    Algorithm::CopyArray(src, src + 5, dest);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(dest[i], i + 1);
    }
}

TEST(Algorithm, TestClamp)
{
    EXPECT_EQ(Algorithm::Clamp(10, 1, 5), 5);
    EXPECT_EQ(Algorithm::Clamp(0, 1, 5), 1);
    EXPECT_EQ(Algorithm::Clamp(3, 1, 5), 3);
}

TEST(Algorithm, TestMax)
{
    EXPECT_EQ(Algorithm::Max(1, 2), 2);
    EXPECT_EQ(Algorithm::Max(2, 1), 2);
    EXPECT_EQ(Algorithm::Max(1, 1), 1);
}

TEST(Algorithm, TestMin)
{
    EXPECT_EQ(Algorithm::Min(1, 2), 1);
    EXPECT_EQ(Algorithm::Min(2, 1), 1);
    EXPECT_EQ(Algorithm::Min(1, 1), 1);
}

TEST(Algorithm, TestIntegerToBinaryString)
{
    char buffer[33];
    
    EXPECT_EQ((Details::IntegerToBinaryString<char, int>(123, buffer)), 7);
    EXPECT_EQ(buffer[32], 0);    
    EXPECT_STREQ(buffer + (33-7-1), "1111011");
}

TEST(Algorithm, TestInt64ToString)
{
    char buffer[24];
    const char* text = Details::IntegerToString<char, int64_t, 10>(-1234567890123456789LL, buffer, FL_ARRAY_COUNTOF(buffer), false);
    EXPECT_EQ(strlen(text), 20);
    EXPECT_STREQ(text, "-1234567890123456789");

    text = Details::IntegerToStringMoved<char, int64_t, 10>(-1234567890123456789LL, buffer, FL_ARRAY_COUNTOF(buffer), false);

    EXPECT_EQ(reinterpret_cast<size_t>(text),reinterpret_cast<size_t>(buffer));
    EXPECT_EQ(strlen(text), 20);
    EXPECT_STREQ(text, "-1234567890123456789");    
}

TEST(Algorithm, TestUInt64ToString)
{
    char buffer[24];
    const char* text = Details::IntegerToString<char, uint64_t, 10>(1234567890123456789ULL, buffer, FL_ARRAY_COUNTOF(buffer), false);
    EXPECT_EQ(strlen(text), 19);    
    EXPECT_STREQ(text, "1234567890123456789");

    const char* MovedText = Details::IntegerToStringMoved<char, uint64_t, 10>(1234567890123456789ULL, buffer, FL_ARRAY_COUNTOF(buffer), false);
    EXPECT_EQ(reinterpret_cast<size_t>(MovedText), reinterpret_cast<size_t>(buffer));
    EXPECT_EQ(strlen(MovedText), 19);
    EXPECT_STREQ(MovedText, "1234567890123456789");
}

TEST(Algorithm, TestUInt64ToStringW)
{
    wchar_t buffer[24];
    const wchar_t* text = Details::IntegerToString<wchar_t, uint64_t, 10>(1234567890123456789ULL, buffer, FL_ARRAY_COUNTOF(buffer), false);
    EXPECT_EQ(wcslen(text), 19);    
    EXPECT_STREQ(text, L"1234567890123456789");

    const wchar_t* MovedText = Details::IntegerToStringMoved<wchar_t, uint64_t, 10>(1234567890123456789ULL, buffer, FL_ARRAY_COUNTOF(buffer), false);
    EXPECT_EQ(reinterpret_cast<size_t>(MovedText), reinterpret_cast<size_t>(buffer));
    EXPECT_EQ(wcslen(MovedText), 19);
    EXPECT_STREQ(MovedText, L"1234567890123456789");
}

TEST(Algorithm, TestDoubleToString)
{
    char buffer[32];
    const char* const Result = Details::DoubleToString<char>(123.456, buffer, FL_ARRAY_COUNTOF(buffer), 3);
    const size_t length = Details::CalculateConvertedStringLength(Result, buffer);
    EXPECT_EQ(length, 7);
    EXPECT_STREQ(Result, "123.456");

    const char* const MovedResult = Details::DoubleToStringMoved<char>(123.456, buffer, FL_ARRAY_COUNTOF(buffer), 3);
    const size_t MovedLength = strlen(MovedResult);
    EXPECT_EQ(MovedLength, 7);
    EXPECT_EQ(reinterpret_cast<size_t>(MovedResult), reinterpret_cast<size_t>(buffer));
    EXPECT_STREQ(MovedResult, "123.456");
}

TEST(Algorithm, TestDoubleToStringW)
{
    wchar_t buffer[32];
    const wchar_t* const Result = Details::DoubleToString<wchar_t>(123.456, buffer, FL_ARRAY_COUNTOF(buffer), 3);
    const size_t length = Details::CalculateConvertedStringLength(Result, buffer);
    EXPECT_EQ(length, 7);
    EXPECT_STREQ(Result, L"123.456");

    const wchar_t* const MovedResult = Details::DoubleToStringMoved<wchar_t>(123.456, buffer, FL_ARRAY_COUNTOF(buffer), 3);
    const size_t MovedLength = wcslen(MovedResult);
    EXPECT_EQ(MovedLength, 7);
    EXPECT_EQ(reinterpret_cast<size_t>(MovedResult), reinterpret_cast<size_t>(buffer));
    EXPECT_STREQ(MovedResult, L"123.456");
}

TEST(Algorithm, TestDoubleToStringGreaterThanThresMax)
{
    constexpr static double ThresMax = (double)(0x7FFFFFFF) + 0.123456;  // NOLINT
    char buffer[64];
    const char* const Result = Details::DoubleToString<char>(ThresMax, buffer, FL_ARRAY_COUNTOF(buffer), 3);
    const size_t length = Details::CalculateConvertedStringLength(Result, buffer);

    // this is not stable on all platforms, so...
    //EXPECT_EQ(length, 12);
    //EXPECT_STREQ(Result, "2.147484e+09");
    EXPECT_TRUE(strstr(Result, "e+") != nullptr);

    const char* const MovedResult = Details::DoubleToStringMoved<char>(ThresMax, buffer, FL_ARRAY_COUNTOF(buffer), 3);
    const size_t MovedLength = strlen(MovedResult);
    EXPECT_TRUE(strstr(MovedResult, "e+") != nullptr);
    EXPECT_EQ(reinterpret_cast<size_t>(MovedResult), reinterpret_cast<size_t>(buffer));
}

TEST(TCharTraits, StringPrintf)
{
    char buffer[100];
    TCharTraits<char>::StringPrintf(buffer, FL_ARRAY_COUNTOF(buffer), "%s %d", "test", 123);
    EXPECT_STREQ(buffer, "test 123");

    wchar_t wbuffer[100];

#if FL_COMPILER_GCC
    const wchar_t* wFmt = L"%S %d";
#else
    const wchar_t* wFmt = L"%s %d";
#endif
    
    TCharTraits<wchar_t>::StringPrintf(wbuffer, FL_ARRAY_COUNTOF(wbuffer), wFmt, L"test", 123);
    EXPECT_EQ(std::wcscmp(wbuffer, L"test 123"), 0);
}

TEST(TCharTraits, CompareN)
{
    EXPECT_EQ(TCharTraits<char>::CompareN("hello111", "hello222", 5), 0);
    EXPECT_EQ(TCharTraits<wchar_t>::CompareN(L"hello333", L"hello444", 5), 0);
}

TEST(TCharTraits, iCompare)
{
    EXPECT_EQ(TCharTraits<char>::iCompare("hello", "HELLO"), 0);
    EXPECT_EQ(TCharTraits<wchar_t>::iCompare(L"hello", L"HELLO"), 0);
}

TEST(TCharTraits, iCompareN)
{
    EXPECT_EQ(TCharTraits<char>::iCompareN("hello111", "HELLO2222", 5), 0);
    EXPECT_EQ(TCharTraits<wchar_t>::iCompareN(L"hello4444", L"HELLO555", 5), 0);
}

TEST(TCharTraits, Find)
{
    EXPECT_STREQ(TCharTraits<char>::Find("hello world", "world"), "world");
    EXPECT_EQ(std::wcscmp(TCharTraits<wchar_t>::Find(L"hello world", L"world"), L"world"), 0);
}

TEST(TCharTraits, iFind)
{
    EXPECT_STREQ(TCharTraits<char>::iFind("hello world", "WORLD"), "world");
    EXPECT_EQ(std::wcscmp(TCharTraits<wchar_t>::iFind(L"hello world", L"WORLD"), L"world"), 0);
}

TEST(TCharTraits, rFind)
{
    EXPECT_STREQ(TCharTraits<char>::rFind("hello world", 'o'), "orld");
    EXPECT_EQ(std::wcscmp(TCharTraits<wchar_t>::rFind(L"hello world", L'o'), L"orld"), 0);
}

TEST(TCharTraits, rFindAny)
{
    EXPECT_STREQ(TCharTraits<char>::rFindAny("hello world", "od"), "d");
    EXPECT_EQ(std::wcscmp(TCharTraits<wchar_t>::rFindAny(L"hello world", L"od"), L"d"), 0);
}

TEST(TCharTraits, rFindNotOfAny)
{
    EXPECT_STREQ(TCharTraits<char>::rFindNotOfAny("hello world", "od"), "ld");
    EXPECT_EQ(std::wcscmp(TCharTraits<wchar_t>::rFindNotOfAny(L"hello world", L"od"), L"ld"), 0);
}

TEST(TCharTraits, Fill)
{
    char buffer[10] = {0};
    TCharTraits<char>::Fill(buffer, 'a', 5);
    EXPECT_STREQ(buffer, "aaaaa");

    wchar_t wbuffer[10] = {0};
    TCharTraits<wchar_t>::Fill(wbuffer, L'a', 5);
    EXPECT_EQ(std::wcscmp(wbuffer, L"aaaaa"), 0);
}
