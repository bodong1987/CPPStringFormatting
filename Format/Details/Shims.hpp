#pragma once

#include <Format/Details/Build.hpp>
#include <Format/Details/Traits.hpp>

namespace FormatLibrary
{
    namespace Details
    {
        /// <summary>
        /// Lengthes the of.
        /// </summary>
        /// <param name="pstr">The PSTR.</param>
        /// <returns>size_t.</returns>
        inline size_t LengthOf(const wchar_t* pstr)
        {
            return TCharTraits<wchar_t>::length(pstr);
        }

        /// <summary>
        /// Lengthes the of.
        /// </summary>
        /// <param name="pstr">The PSTR.</param>
        /// <returns>size_t.</returns>
        inline size_t LengthOf(const char* pstr)
        {
            return TCharTraits<char>::length(pstr);
        }

        /// <summary>
        /// Lengthes the of.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns>size_t.</returns>
        inline size_t LengthOf(const std::basic_string<char>& str)
        {
            return str.length();
        }

        /// <summary>
        /// Lengthes the of.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns>size_t.</returns>
        inline size_t LengthOf(const std::basic_string<wchar_t>& str)
        {
            return str.length();
        }

        /// <summary>
        /// PTRs the of.
        /// </summary>
        /// <param name="pstr">The PSTR.</param>
        /// <returns>const wchar_t *.</returns>
        inline const wchar_t* PtrOf(const wchar_t* pstr)
        {
            return pstr;
        }

        /// <summary>
        /// PTRs the of.
        /// </summary>
        /// <param name="pstr">The PSTR.</param>
        /// <returns>const char *.</returns>
        inline const char* PtrOf(const char* pstr)
        {
            return pstr;
        }

        /// <summary>
        /// PTRs the of.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns>const char *.</returns>
        inline const char* PtrOf(const std::basic_string<char>& str)
        {
            return str.c_str();
        }

        /// <summary>
        /// PTRs the of.
        /// </summary>
        /// <param name="str">The string.</param>
        /// <returns>const wchar_t *.</returns>
        inline const wchar_t* PtrOf(const std::basic_string<wchar_t>& str)
        {
            return str.c_str();
        }
    }    
}
