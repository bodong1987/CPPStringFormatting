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

#if FL_COMPILER_IS_GREATER_THAN_CXX11
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

TEST(TAutoArray, Shrink)
{
    TAutoArray<int, 0xF> array;
    for (int i = 0; i < 100; ++i)
    {
        array.AddItem(i);
    }
    EXPECT_EQ(array.GetLength(), 100);
    size_t initialAllocatedCount = array.GetAllocatedCount();
    EXPECT_GT(initialAllocatedCount, 100);

    array.Shrink();
    EXPECT_EQ(array.GetLength(), 100);
    EXPECT_EQ(array.GetAllocatedCount(), 100);

    for (int i = 0; i < 100; ++i)
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

    str.AddAlignStr("Hello", strlen("Hello"), 10, false, ' ');
    EXPECT_STREQ(str.CStr(), "Hello     ");

    str.AddAlignStr("World", strlen("World"), 10, true, '$');
    EXPECT_STREQ(str.CStr(), "Hello     $$$$$World");
    
    str.AddAlignStr("Hi", strlen("Hi"), 5, false, '0');
    EXPECT_STREQ(str.CStr(), "Hello     $$$$$WorldHi000");

    str.AddAlignStr("HelloWorld", strlen("HelloWorld"), 5, true, '0');
    EXPECT_STREQ(str.CStr(), "Hello     $$$$$WorldHi000HelloWorld");

    str.AddAlignStr("12345", strlen("12345"), 5, true, '0');
    EXPECT_STREQ(str.CStr(), "Hello     $$$$$WorldHi000HelloWorld12345");
}

TEST(TAutoString, AddAlignWStr)
{
    TAutoString<wchar_t> str;

    str.AddAlignStr(L"Hello", wcslen(L"Hello"), 10, false, ' ');
    EXPECT_STREQ(str.CStr(), L"Hello     ");

    str.AddAlignStr(L"World", wcslen(L"World"), 10, true, '$');
    EXPECT_STREQ(str.CStr(), L"Hello     $$$$$World");
    
    str.AddAlignStr(L"Hi", wcslen(L"Hi"), 5, false, '0');
    EXPECT_STREQ(str.CStr(), L"Hello     $$$$$WorldHi000");

    str.AddAlignStr(L"HelloWorld", wcslen(L"HelloWorld"), 5, true, '0');
    EXPECT_STREQ(str.CStr(), L"Hello     $$$$$WorldHi000HelloWorld");

    str.AddAlignStr(L"12345", wcslen(L"12345"), 5, true, '0');
    EXPECT_STREQ(str.CStr(), L"Hello     $$$$$WorldHi000HelloWorld12345");
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
