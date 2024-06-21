#pragma once

namespace FormatLibrary
{
    class Noncopyable
    {
    public:
        Noncopyable() {}        
        ~Noncopyable() {}
    protected:
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator = (const Noncopyable&) = delete;
    };
}
