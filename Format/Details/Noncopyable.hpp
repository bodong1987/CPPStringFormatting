#pragma once

#include <Format/Details/Build.hpp>

namespace FormatLibrary
{
    /// <summary>
    /// Class Noncopyable.
    /// </summary>
    class Noncopyable
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Noncopyable"/> class.
        /// </summary>
        Noncopyable() {}
        /// <summary>
        /// Finalizes an instance of the <see cref="Noncopyable"/> class.
        /// </summary>
        ~Noncopyable() {}
    protected:
#if FL_COMPILER_WITH_CXX11
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator = (const Noncopyable&) = delete;
#else
    private:
        /// <summary>
        /// Initializes a new instance of the <see cref="Noncopyable"/> class.
        /// </summary>
        /// <param name="">The .</param>
        Noncopyable(const Noncopyable&);
        /// <summary>
        /// Operator=s the specified .
        /// </summary>
        /// <param name="">The .</param>
        /// <returns>FormatLibrary.Utility.Noncopyable &.</returns>
        Noncopyable& operator = (const Noncopyable&);
#endif
    };
}
