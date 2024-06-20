#include <gtest/gtest.h>
#include <Format/Format.hpp>
#include <Format/StandardLibraryAdapter.hpp>

using namespace FormatLibrary;

TEST(Format, StandardLibrary)
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
    EXPECT_EQ(r5, "99");

    const std::string r6 = StandardLibrary::Format("0x{0:x}", 100, (unsigned long)(100));
    EXPECT_EQ(r6, "0x64");

    const std::wstring r7 = StandardLibrary::Format(L"Test{1}, {2:f4}, {0}, {0,4}", L" X ", 20, -10.005f);
    EXPECT_EQ(r7, L"Test20, -10.0050,  X ,  X ");

    const std::wstring r8 = StandardLibrary::Format(L"Test{1}, {2:f4}, {0}, {0,4}");
    EXPECT_EQ(r8, L"Test{1}, {2:f4}, {0}, {0,4}");

    const std::wstring r9 = StandardLibrary::Format(std::wstring(L"Test{1}, {2:f4}, {0}, {0,4}"), L" X ", 20, -10.005f);
    EXPECT_EQ(r9, L"Test20, -10.0050,  X ,  X ");

    const std::string r10 = StandardLibrary::Format(std::string("{0}--#--{1,8}--#--{3}"), 100, -40.2f, std::string("xxx"));
    EXPECT_EQ(r10, "100--#--  -40.20--#--{3}");

    const std::wstring r11 = StandardLibrary::Format(L"\u4F60\u597D : {0}", L"\u4E2D\u6587");
    EXPECT_EQ(r11, L"\u4F60\u597D : \u4E2D\u6587");
}

