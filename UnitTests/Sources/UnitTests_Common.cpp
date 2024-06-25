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

#if FL_COMPILER_WITH_CXX11
TEST(TAutoArray, MoveConstructor)
{
    TAutoArray<int> array1;
    for (int i = 0; i < 10; ++i)
    {
        array1.AddItem(i);
    }
    TAutoArray<int> array2(FL_MOVE_SEMANTIC(array1));
    EXPECT_EQ(array2.GetLength(), 10);
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(array2[i], i);
    }
    EXPECT_EQ(array1.GetLength(), 0);
}
#endif

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

TEST(TAutoString, AddLongStr)
{    
    TAutoString<char> str;
    str.AddStr("Hello");
    str.AddStr(", ");
    str.AddStr("World");

    std::string longStr(1024, 'A');
    std::string longStr2(2048, 'B');

    str.AddStr(longStr.c_str());
    str.AddStr(longStr2.c_str());
    
    EXPECT_STREQ(str.CStr(), (std::string("Hello, World") + longStr + longStr2).c_str());
}

TEST(TAutoString, AddAlignStr)
{
    TAutoString<char> str;

    str.AddAlignStr("Hello", nullptr, 10, false, ' ');
    EXPECT_STREQ(str.CStr(), "Hello     ");

    str.AddAlignStr("World", nullptr, 10, true, '$');
    EXPECT_STREQ(str.CStr(), "Hello     $$$$$World");
    
    str.AddAlignStr("Hi", nullptr, 5, false, '0');
    EXPECT_STREQ(str.CStr(), "Hello     $$$$$WorldHi000");

    str.AddAlignStr("HelloWorld", nullptr, 5, true, '0');
    EXPECT_STREQ(str.CStr(), "Hello     $$$$$WorldHi000HelloWorld");

    str.AddAlignStr("12345", nullptr, 5, true, '0');
    EXPECT_STREQ(str.CStr(), "Hello     $$$$$WorldHi000HelloWorld12345");
}

TEST(TAutoString, Clear)
{
    TAutoString<wchar_t> str;

    str.AddStr(L"Hello");
    EXPECT_STREQ(str.CStr(), L"Hello");

    str.Clear();

    EXPECT_TRUE(str.IsEmpty());
    EXPECT_STREQ(str.CStr(), L"");
}
