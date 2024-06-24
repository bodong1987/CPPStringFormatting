#include <Format/Common/Build.hpp>

#if FL_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable:4616)
#endif
#include <gtest/gtest.h>
#if FL_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

#include <Format/Common/AutoString.hpp>

using namespace Formatting;


TEST(TAutoArray, DefaultConstructor)
{
    TAutoArray<int> array;
    EXPECT_EQ(array.GetLength(), 0);
}

TEST(TAutoArray, AddItem)
{
    TAutoArray<int> array;
    for (int i = 0; i < 10; ++i)
    {
        array.AddItem(i);
    }
    EXPECT_EQ(array.GetLength(), 10);
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(array[i], i);
    }
}

TEST(TAutoArray, CopyConstructor)
{
    TAutoArray<int> array1;
    for (int i = 0; i < 10; ++i)
    {
        array1.AddItem(i);
    }
    TAutoArray<int> array2(array1);
    EXPECT_EQ(array2.GetLength(), 10);
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(array2[i], i);
    }
}

TEST(TAutoArray, MoveConstructor)
{
    TAutoArray<int> array1;
    for (int i = 0; i < 10; ++i)
    {
        array1.AddItem(i);
    }
    TAutoArray<int> array2(std::move(array1));
    EXPECT_EQ(array2.GetLength(), 10);
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(array2[i], i);
    }
    EXPECT_EQ(array1.GetLength(), 0);
}

TEST(TAutoArray, ExpandHeapSpace)
{
    TAutoArray<int> array;
    for (int i = 0; i < 1000; ++i)
    {
        array.AddItem(i);
    }
    EXPECT_EQ(array.GetLength(), 1000);
    for (int i = 0; i < 1000; ++i)
    {
        EXPECT_EQ(array[i], i);
    }
}

TEST(TAutoString, DefaultConstructor)
{
    TAutoString<char> str;
    EXPECT_STREQ(str.CStr(), "");
}

TEST(TAutoString, ConstructorWithCString)
{
    TAutoString<char> str("Hello");
    EXPECT_STREQ(str.CStr(), "Hello");
}

TEST(TAutoString, AddChar)
{
    TAutoString<char> str;
    str.AddChar('H');
    str.AddChar('i');
    EXPECT_STREQ(str.CStr(), "Hi");
}

TEST(TAutoString, AddStr)
{
    TAutoString<char> str;
    str.AddStr("Hello");
    str.AddStr(", ");
    str.AddStr("World");
    EXPECT_STREQ(str.CStr(), "Hello, World");
}

TEST(TAutoString, InjectAdd)
{
    TAutoString<char> str;
    str.AddChar('H');
    str.InjectAdd(1);
    EXPECT_EQ(str.CStr()[1], '\0');
}
