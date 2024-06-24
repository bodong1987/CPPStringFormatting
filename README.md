# CPPFormatLibrary
这是一个使用C#格式化字符串风格来格式化C++字符串的库，它是类型安全的，可乱序的格式化库。并非所有的C#格式化字符串的格式描述符都完美支持，它只支持大部分常见的格式化方法。  
This is a library that uses the C# formatted string style to format C++ strings. It is a type-safe, reorderable formatting library. Not all C# format string format descriptors are perfectly supported, it only supports most common formatting methods.  

本项目支持几乎常见所有主流编译，无论有无C++ 11支持都可以。我使用Visual Studio 2008/2010/2022、CodeBlock with gcc、xcode 15对项目进行了测试，并通过了测试。  
This project supports almost all common mainstream compilations, whether with or without C++11 support. I tested the project using Visual Studio 2008/2010/2022, CodeBlock with gcc, xcode 15 and it passed.  

## How to use?
这是一个纯C++头文件项目，clone项目，然后将项目根目录添加到头文件搜索路径中并包含`Format/Format.hpp`即可。  

This is a pure C++ header file project, clone the project, then add the project root directory to the header file search path and include `Format/Format.hpp`.  

## How to Test?
请先Clone本项目，然后使用CMake生成项目即可测试。
需要注意的一点是本项目依赖于googletests，所以你在clone之后，需要使用如下指令确保googletests的submodule正确被拉取下来了，否则CMake生成项目时会出错。  

```bat
git submodule init
git submodule update
```

Please clone this project first, and then use CMake to generate the project for testing.
One thing to note is that this project depends on googletests, so after cloning, you need to use the following instructions to ensure that the submodule of googletests is correctly pulled down, otherwise CMake will make an error when generating the project.  

另外如果你想要测试的C++编译器版本很早，比如不支持C++ 11的`Visual Studio 2008`，那么你可能需要手动将googletests切换到更早的tag上，比如`1.8.x`。  
In addition, if the C++ compiler version you want to test is very early, such as Visual Studio 2008 that does not support C++ 11, then you may need to manually switch googletests to an earlier tag, such as `1.8.x`.  

生成项目后，使用IDE打开生成的项目文件即可，编译和运行`UniTests`项目即可开始测试。`UnitTests_Format.cpp`中代码即是针对`std::basic_string<TCharType>`的使用案例。你可以参考StandardLibraryAdapter.hpp的代码为你自己的字符串类添加适配器，即可让这套系统配合你的字符串类进行工作。  

After generating the project, just use the IDE to open the generated project file, compile and run the `UniTests` project to start testing. The code in `UnitTests_Format.cpp` is a use case for `std::basic_string<TCharType>`. You can refer to the code of StandardLibraryAdapter.hpp to add an adapter to your own string class, so that this system can work with your string class.  

## Code Segment
这里简单展示一下代码风格和用法，你可以在`UnitTests_Format.cpp`中找到更多用法和案例。  

Here is a brief demonstration of the code style and usage. You can find more usage and cases in `UnitTests_Format.cpp`.   

```C++
#include <Format/Format.hpp>
#include <Format/StandardLibraryAdapter.hpp>

using namespace FormatLibrary;

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
```

