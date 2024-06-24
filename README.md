# CPPFormatLibrary
这是一个使用C#格式化字符串风格来格式化C++字符串的库，它是类型安全的，可乱序的格式化库。并非所有的C#格式化字符串的格式描述符都完美支持，它只支持大部分常见的格式化方法。  
This is a library that uses the C# formatted string style to format C++ strings. It is a type-safe, reorderable formatting library. Not all C# format string format descriptors are perfectly supported, it only supports most common formatting methods.  

本项目支持几乎常见所有主流编译，无论有无C++ 11支持都可以。我使用Visual Studio 2008/2010/2022、CodeBlock with gcc、xcode 15对项目进行了测试，并通过了测试。  
This project supports almost all common mainstream compilations, whether with or without C++11 support. I tested the project using Visual Studio 2008/2010/2022, CodeBlock with gcc, xcode 15 and it passed.  

## 如何使用？ How to use?
这是一个纯C++头文件项目，clone项目，然后将项目根目录添加到头文件搜索路径中并包含`Format/Format.hpp`即可。  

This is a pure C++ header file project, clone the project, then add the project root directory to the header file search path and include `Format/Format.hpp`.  

## 如何测试？ How to Test?
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

## 代码片段 Code Segment
这里简单展示一下代码风格和用法，你可以在`UnitTests_Format.cpp`中找到更多用法和案例。  

Here is a brief demonstration of the code style and usage. You can find more usage and cases in `UnitTests_Format.cpp`.   

```C++
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

## 如何集成？ How to integrated
想要将格式化库适配你自己的字符串类或者使用你自己的容器类来接管格式化库内部的容器，那么你只需要做三件事：
* 第一，实现自己的Policy类，这个类需要告知框架必须要的基础类型都是什么，这通过typedef来实现；并且你还需要实现几个基础接口即可：`FindByHashKey`、`ReserveList`、`AppendPattern`。  
* 第二，你需要在命名空间`Formatting::Shims`下实现两个垫片函数`PtrOf`和`LengthOf`，这两个函数是针对你的字符串类的重载。  
* 第三，你需要实现自己的`Format`函数。如果你使用C++ 11或更新标准，那么你只需要使用不定参数模板即可；如果你要支持C++ 11更早的版本，你需要用到一些宏技巧来生成支持多个参数的`Format`函数。  
你可以参考`UnitTests/Sources/MFCAdapter.hpp`的源代码，这里展示了如何为MFC字符串提供Format支持。当然也可以参考`Format/StandardLibraryAdapter.hpp`，这里是针对`stl::basic_string<TCharType>`的适配代码。  
通过这几个简单的适配器，你就可以将这个字符串格式化库用到你自己的类型上了。  

If you want to adapt the formatting library to your own string class or use your own container class to take over the container inside the formatting library, then you only need to do three things:
* First, implement your own Policy class. This class needs to tell the framework what the basic types are. This is achieved through typedef; and you also need to implement several basic interfaces: `FindByHashKey`, `ReserveList`, `AppendPattern`.  
* Second, you need to implement two shim functions `PtrOf` and `LengthOf` under the namespace `Formatting::Shims`. These two functions are overloaded for your string class.  
* Third, you need to implement your own `Format` function. If you use C++11 or newer standards, then you only need to use indefinite parameter templates; if you want to support earlier versions of C++11, you need to use some macro tricks to generate a `Format` function that supports multiple parameters.  
You can refer to the source code of `UnitTests/Sources/MFCAdapter.hpp`, which shows how to provide Format support for MFC strings. Of course, you can also refer to `Format/StandardLibraryAdapter.hpp`, here is the adaptation code for `stl::basic_string<TCharType>`.  
With these simple adapters, you can apply this string formatting library to your own types.  

## 如何扩展？ How to extends?
CPPFormatLibrary本身是类型安全的系统，如果你尝试将不支持的类型提交给格式化函数，那么你将会在编译阶段得到一个编译错误。要解决这个错误，你有两种办法：
* 第一，将参数转换成受支持的类型。
* 第二，提供一个针对该类型的自定义Translator，这样框架将能自动帮你完成转换操作。
你可以在`UnitTests/Sources/UnityTests_Extends.cpp`中找到扩展的方法和举例。   

CPPFormatLibrary itself is a type-safe system, if you try to submit an unsupported type to the formatting function, then you will get a compilation error during the compilation phase. To resolve this error, you have two options:
* First, convert the parameters to a supported type.
* Second, provide a custom Translator for this type, so that the framework will automatically complete the conversion operation for you.
You can find extension methods and examples in `UnitTests/Sources/UnityTests_Extends.cpp`.  

```C++
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
```

## 差异 Difference
虽然CPPFormatLibrary支持了大部分常见的格式化需求，但并也有许多C#格式化的功能尚不支持。具体C#支持的格式化规范请参考这里:  https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-numeric-format-strings  

Although CPPFormatLibrary supports most common formatting needs, there are also many C# formatting functions that are not yet supported. For specific formatting specifications supported by C#, please refer here:  
https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-numeric-format-strings  


| Type       | Status
| :----      | :------:            
| B          | &#x2714;            
| C          | &#x2716;            
| D          | &#x2714;            
| E          | &#x2714;            
| F          | &#x2714;            
| G          | &#x2716;            
| N          | &#x2716;            
| P          | &#x2716;            
| R          | &#x2716;            
| X          | &#x2714;            
| Percision  | &#x2714;            
| Width      | &#x2714;          

遇到不支持的格式标志符时C#会抛出异常，CPPFormatLibrary会直接输出格式符字符串，一些情况会触发assert或忽略。因此当你遇到与预期不一致的问题时，请检查格式标志符是否正确，或者是否使用了不被支持的标志符。  

C# will throw an exception when encountering an unsupported format identifier, and CPPFormatLibrary will directly output the format character string. In some cases, assert or ignore will be triggered. So when you encounter problems that are inconsistent with expectations, please check whether the format identifier is correct, or whether an unsupported identifier is used.  

## 提交错误报告 Bugreport
直接通过[Issues](https://github.com/bodong1987/CPPFormatLibrary/issues)页面提交即可  

Submit directly through the [Issues](https://github.com/bodong1987/CPPFormatLibrary/issues) page  
