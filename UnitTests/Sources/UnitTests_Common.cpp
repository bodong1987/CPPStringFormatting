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

TEST(TAutoArray, AddItems)
{
    TAutoArray<int> array;
    constexpr int itemCount = 1000;
    int items[itemCount];

    // Initialize items array
    for (int i = 0; i < itemCount; ++i)
    {
        items[i] = i;
    }

    // Add range of items
    array.AddItems(items, itemCount);

    // Verify the length of the array
    EXPECT_EQ(array.GetLength(), itemCount);

    // Verify the contents of the array
    for (int i = 0; i < itemCount; ++i)
    {
        EXPECT_EQ(array[i], items[i]);
    }
}

TEST(TAutoArray, AddItemsMoreComplex)
{
    TAutoArray<int> array;
    constexpr int itemCount0 = 128; 
    constexpr int itemCount1 = 10000;
    int items0[itemCount0];
    int items1[itemCount1];

    // Initialize items array
    // ReSharper disable once CppUseStdSize
    for (int i = 0; i < FL_ARRAY_COUNTOF(items0); ++i)  // NOLINT(clang-diagnostic-sign-compare)
    {
        items0[i] = i;
    }

    // ReSharper disable once CppUseStdSize
    for (int i = 0; i < FL_ARRAY_COUNTOF(items1); ++i)  // NOLINT(clang-diagnostic-sign-compare)
    {
        items1[i] = i;
    }

    // Add range of items
    array.AddItems(items0, itemCount0);
    array.AddItems(items1, itemCount1);

    // Verify the length of the array
    EXPECT_EQ(array.GetLength(), itemCount0 + itemCount1);

    // Verify the contents of the array
    for (int i = 0; i < itemCount0; ++i)
    {
        EXPECT_EQ(array[i], items0[i]);
    }

    for (int i = itemCount0; i < itemCount0 + itemCount1; ++i)
    {
        EXPECT_EQ(array[i], items1[i - itemCount0]);
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

TEST(TAutoArray, BeginEnd)
{
    TAutoArray<int, 0xF> array;
    for (int i = 0; i < 100; ++i)
    {
        array.AddItem(i);
    }

    int index = 0;
    for (TAutoArray<int, 0xF>::iterator it = array.begin(); it != array.end(); ++it)
    {
        EXPECT_EQ(*it, index);
        ++index;
    }
    EXPECT_EQ(index, 100);
}

TEST(TAutoArray, RBeginREnd)
{
    TAutoArray<int, 0xF> array;
    for (int i = 0; i < 100; ++i)
    {
        array.AddItem(i);
    }

    int index = 99;
    for (TAutoArray<int, 0xF>::reverse_iterator it = array.rbegin(); it != array.rend(); ++it)
    {
        EXPECT_EQ(*it, index);
        --index;
    }
    EXPECT_EQ(index, -1);
}

#if FL_COMPILER_IS_GREATER_THAN_CXX11
TEST(TAutoArray, RangeBasedFor)
{
    TAutoArray<int, 0xF> array;
    for (int i = 0; i < 100; ++i)
    {
        array.AddItem(i);
    }

    int index = 0;
    for (const auto& item : array)
    {
        EXPECT_EQ(item, index);
        ++index;
    }
    EXPECT_EQ(index, 100);
}
#endif

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

    const std::string longStr(1024, 'A');
    const std::string longStr2(2048, 'B');

    str.AddStr(longStr.c_str(), longStr.length());
    str.AddStr(longStr2.c_str(), longStr2.length());
    
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

TEST(TAutoString, BeginEnd)
{
    TAutoString<char> str("Hello, World!");
    const char* expected = "Hello, World!";
    int index = 0;

    for (TAutoString<char>::iterator it = str.begin(); it != str.end(); ++it)
    {
        EXPECT_EQ(*it, expected[index]);
        ++index;
    }
    EXPECT_EQ(index, strlen(expected));
}

TEST(TAutoString, RBeginREnd)
{
    TAutoString<char> str("Hello, World!");
    const char* expected = "!dlroW ,olleH";
    int index = 0;

    for (TAutoString<char>::reverse_iterator it = str.rbegin(); it != str.rend(); ++it)
    {
        EXPECT_EQ(*it, expected[index]);
        ++index;
    }
    EXPECT_EQ(index, strlen(expected));
}

#if FL_COMPILER_IS_GREATER_THAN_CXX11
TEST(TAutoString, RangeBasedFor)
{
    TAutoString<char> str("Hello, World!");
    const char* expected = "Hello, World!";
    int index = 0;

    for (const auto& ch : str)
    {
        EXPECT_EQ(ch, expected[index]);
        ++index;
    }
    EXPECT_EQ(index, strlen(expected));
}
#endif