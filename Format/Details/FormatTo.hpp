// FormatLibrary 
// @author bodong 
// this is the wrapper of Format To impl
#pragma once

#include <Format/Details/Build.hpp>

namespace FormatLibrary
{
    namespace Details
    {
#if !FL_COMPILER_WITH_CXX11
#define _FL_FORMAT_TO_INDEX_ 1
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 2
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 3
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 4
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 5
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 6
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 7
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 8
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 9
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 10
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 11
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 12
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 13
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 14
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_

#define _FL_FORMAT_TO_INDEX_ 15
#include <Format/Details/BreforeCPP11Inlines/FormatToInner.hpp>
#undef _FL_FORMAT_TO_INDEX_
#else
    
#endif
    }
}