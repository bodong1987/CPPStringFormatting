#include <Format/Common/Build.hpp>

#if FL_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable:4616)
#endif
#include <gtest/gtest.h>
#if FL_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

#include "MFCAdapter.hpp"

#if FL_PLATFORM_WINDOWS

using namespace Formatting;

TEST(MFCFormat, MFC_CString)
{
    EXPECT_EQ(MFC::Format(_T("Hello CppMiniToolkit")), _T("Hello CppMiniToolkit"));
    EXPECT_EQ(MFC::Format(_T("Hello CppMiniToolkit {0}"), 1024), _T("Hello CppMiniToolkit 1024"));    

    EXPECT_EQ(MFC::Format(_T("{0} {1}"), 123, _T("hello")), _T("123 hello"));
    EXPECT_EQ(MFC::Format(_T("{1} {0}"), 123, _T("hello")), _T("hello 123"));
    EXPECT_EQ(MFC::Format(_T("{0} {0}"), 123), _T("123 123"));
    EXPECT_EQ(MFC::Format(_T("{0}"), _T("hello")), _T("hello"));
    EXPECT_EQ(MFC::Format(_T("{0}"), _T('a')), _T("a"));
    EXPECT_EQ(MFC::Format(_T("{0}"), true), _T("True"));
    EXPECT_EQ(MFC::Format(_T("{0}"), false), _T("False"));
    EXPECT_EQ(MFC::Format(_T(""), 123), _T(""));
    EXPECT_EQ(MFC::Format(_T("hello")), _T("hello"));
    EXPECT_EQ(MFC::Format(_T("{0} {1} {2}"), 123, _T("hello"), 1.23), _T("123 hello 1.23"));
}

#endif