#pragma once

#include <Format/Common/Build.hpp>

namespace FormatLibrary
{
    class Noncopyable
    {
    public:
        Noncopyable() {}        
        ~Noncopyable() {}
#if FL_COMPILER_WITH_CXX11
    protected:
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator = (const Noncopyable&) = delete;
#else
	private:
		// declare without implementation
		Noncopyable(const Noncopyable&);
		Noncopyable& operator = (const Noncopyable&);
#endif
    };
}
