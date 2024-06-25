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
    char buffer[21];
    EXPECT_EQ(Details::Int64ToString<char>(1234567890123456789LL, buffer, 10, false), 19);
    EXPECT_STREQ(buffer, "1234567890123456789");
}

TEST(Algorithm, TestUInt64ToString)
{
    char buffer[21];
    EXPECT_EQ(Details::UInt64ToString<char>(1234567890123456789ULL, buffer, 10, false), 19);
    EXPECT_STREQ(buffer, "1234567890123456789");
}

TEST(Algorithm, TestDoubleToString)
{
    char buffer[32];
    EXPECT_EQ(Details::DoubleToString<char>(123.456, buffer, sizeof(buffer), 3), 7);
    EXPECT_STREQ(buffer, "123.456");
}
