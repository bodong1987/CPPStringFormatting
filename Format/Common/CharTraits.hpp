/*
    MIT License

    Copyright (c) 2024 CPPStringFormatting

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    Project URL: https://github.com/bodong1987/CPPStringFormatting
*/
#pragma once

#include <cstddef>
#include <cstdarg>
#include <string>

#include <Format/Common/Build.hpp>

namespace Formatting
{
    template < typename TCharType >
    class TCharTraits :
        public std::char_traits<TCharType>
    {
    };

    template <>
    class TCharTraits<char> :
        public std::char_traits<char>
    {
    private:
        // NOLINTBEGIN
        static const char* strrstr(const char* haystack, const char* needle)
        {
            const char* r = nullptr;

            if (!needle[0])
            {
                return haystack + strlen(haystack);
            }

            while (true)
            {
                const char* p = strstr(haystack, needle);

                if (!p)
                {
                    return r;
                }

                r = p;
                haystack = p + 1;
            }

            // return nullptr;
        }

        /*
         * Find the first occurrence of find in s, ignore case.
         */
        static const char* strcasestr(const char* s, const char* find)
        {
            char c, sc;
            size_t len;

            if ((c = *find++) != 0)
            {
                c = (char)tolower(c);
                len = strlen(find);
                do
                {
                    do
                    {
                        if ((sc = *s++) == 0)
                        {
                            return nullptr;
                        }
                    } while (tolower(sc) != c);
                } while (iCompareN(s, find, len) != 0);
                s--;
            }
            return s;
        }
        // NOLINTEND

    public:
        static int32_t StringPrintf(
            char* string,
            size_t sizeInBytes,
            const char* format,
            ...
        )
        {
            va_list arglist; // NOLINT
            va_start(arglist, format);

            int result = 0;
#if FL_COMPILER_MSVC
            result = _vsprintf_s_l(string, sizeInBytes, format, nullptr, arglist);
#else
            result = vsnprintf(string, sizeInBytes, format, arglist);
#endif
            va_end(arglist);

            return result;
        }

        static const char* StaticEFormat()
        {
            return "%e";
        }

        static char StaticPathSeparator()
        {
#if FL_PLATFORM_WINDOWS
            return '\\';
#else
            return '/';
#endif
        }

        static int Compare(const char* first, const char* second)
        {
            return strcmp(first, second);
        }

        static int CompareN(const char* first, const char* second, std::size_t n)
        {
            return strncmp(first, second, n);
        }

        static int iCompare(const char* first, const char* second)
        {
#if FL_COMPILER_MSVC
            return _stricmp(first, second);
#else
            return strcasecmp(first, second);
#endif
        }

        static int iCompareN(const char* first, const char* second, std::size_t n)
        {
#if FL_COMPILER_MSVC
            return _strnicmp(first, second, n);
#else
            return strncasecmp(first, second, n);
#endif
        }

        static const char* Find(const char* str, const char* match)
        {
            return strstr(str, match);
        }

        static const char* iFind(const char* str, const char* match)
        {
            return strcasestr(str, match);
        }

        static const char* rFind(const char* str, char ch)
        {
            return strrchr(str, ch);
        }

        static const char* rFind(const char* str, const char* subString)
        {
            return strrstr(str, subString);
        }

        static const char* rFindAny(const char* str, const char* targets)
        {
            for (const char* p = str + strlen(str) - 1; p >= str; --p)
            {
                if(strchr(targets, *p) != nullptr)
                {
                    return p;
                }
            }

            return nullptr;
        }

        static const char* rFindNotOfAny(const char* str, const char* targets)
        {
            for (const char* p = str + strlen(str) - 1; p >= str; --p)
            {
                if (strchr(targets, *p) == nullptr)
                {
                    return p;
                }
            }

            return nullptr;
        }

        static char* Fill(char* dest, char val, size_t length)
        {
            return (char*)memset(dest, val, length); // NOLINT
        }

        // NOLINTBEGIN
        static int IsAlnum(char ch)
        {
            return isalnum(ch);
        }

        static int IsAlpha(char ch)
        {
            return isalpha(ch);
        }

        static int IsLower(char ch)
        {
            return islower(ch);
        }

        static int IsUpper(char ch)
        {
            return isupper(ch);
        }

        static int IsDigit(char ch)
        {
            return isdigit(ch);
        }

        static int IsXDigit(char ch)
        {
            return isxdigit(ch);
        }

        static int IsCntrl(char ch)
        {
            return iscntrl(ch);
        }

        static int IsGraph(char ch)
        {
            return isgraph(ch);
        }

        static int IsPrint(char ch)
        {
            return isprint(ch);
        }

        static int IsPunct(char ch)
        {
            return ispunct(ch);
        }

        static int IsSpace(char ch)
        {
            return isspace(ch);
        }

        static int ToLower(char ch)
        {
            return tolower(ch);
        }

        static int ToUpper(char ch)
        {
            return toupper(ch);
        }
        // NOLINTEND

        constexpr static char GetSpace()
        {
            return ' ';
        }

        constexpr static char GetZero()
        {
            return '0';
        }

        constexpr static char GetEndFlag()
        {
            return 0;
        }
    };

    template <>
    class TCharTraits<wchar_t> :
        public std::char_traits<wchar_t>
    {
    private:
        // NOLINTBEGIN
        static const wchar_t* wcsrstr(const wchar_t* haystack, const wchar_t* needle)
        {
            const wchar_t* r = nullptr;

            if (!needle[0])
            {
                return haystack + wcslen(haystack);
            }

            while (true)
            {
                const wchar_t* p = wcsstr(haystack, needle);

                if (!p)
                {
                    return r;
                }

                r = p;
                haystack = p + 1;
            }

            // return nullptr;
        }

        /*
         * Find the first occurrence of find in s, ignore case.
         */
        static const wchar_t* wcscasestr(const wchar_t* s, const wchar_t* find)
        {
            wchar_t c, sc;
            size_t len;

            if ((c = *find++) != 0)
            {
                c = (wchar_t)tolower(c);
                len = wcslen(find);
                do
                {
                    do
                    {
                        if ((sc = *s++) == 0)
                        {
                            return nullptr;
                        }
                    } while (tolower(sc) != c);
                } while (iCompareN(s, find, len) != 0);
                s--;
            }
            return s;
        }
        // NOLINTEND
    public:
        static int32_t StringPrintf(
            wchar_t* string,
            size_t sizeInWords,
            const wchar_t* format,
            ...
        )
        {
            va_list arglist; // NOLINT

            va_start(arglist, format);

            int result = 0;

#if FL_COMPILER_MSVC
            result = _vswprintf_s_l(string, sizeInWords, format, nullptr, arglist);
#elif FL_COMPILER_GCC && FL_PLATFORM_WINDOWS
            result = vswprintf(string, format, arglist);
#else
            result = vswprintf(string, sizeInWords, format, arglist);
#endif
            va_end(arglist);

            return result;
        }

        static const wchar_t* StaticEFormat()
        {
            return L"%e";
        }

        static wchar_t StaticPathSeparator()
        {
#if FL_PLATFORM_WINDOWS
            return L'\\';
#else
            return L'/';
#endif
        }

        static int Compare(const wchar_t* first, const wchar_t* second)
        {
            return wcscmp(first, second);
        }

        static int CompareN(const wchar_t* first, const wchar_t* second, std::size_t n)
        {
            return wcsncmp(first, second, n);
        }

        static int iCompare(const wchar_t* first, const wchar_t* second)
        {
#if FL_COMPILER_MSVC
            return _wcsicmp(first, second);
#elif FL_COMPILER_GCC && FL_PLATFORM_WINDOWS
            return wcsicmp(first, second);
#else
            return wcscasecmp(first, second);
#endif
        }

        static int iCompareN(const wchar_t* first, const wchar_t* second, std::size_t n)
        {
#if FL_COMPILER_MSVC
            return _wcsnicmp(first, second, n);
#elif FL_COMPILER_GCC && FL_PLATFORM_WINDOWS
            return wcsnicmp(first, second, n);
#else
            return wcsncasecmp(first, second, n);
#endif
        }

        static const wchar_t* Find(const wchar_t* str, const wchar_t* match)
        {
            return wcsstr(str, match);
        }

        static const wchar_t* iFind(const wchar_t* str, const wchar_t* match)
        {
            return wcscasestr(str, match);
        }

        static const wchar_t* rFind(const wchar_t* str, wchar_t ch)
        {
            return wcsrchr(str, ch);
        }

        static const wchar_t* rFind(const wchar_t* str, const wchar_t* subString)
        {
            return wcsrstr(str, subString);
        }

        static const wchar_t* rFindAny(const wchar_t* str, const wchar_t* targets)
        {
            for (const wchar_t* p = str + wcslen(str) - 1; p >= str; --p)
            {
                if (wcschr(targets, *p) != nullptr)
                {
                    return p;
                }
            }

            return nullptr;
        }

        static const wchar_t* rFindNotOfAny(const wchar_t* str, const wchar_t* targets)
        {
            for (const wchar_t* p = str + wcslen(str) - 1; p >= str; --p)
            {
                if (wcschr(targets, *p) == nullptr)
                {
                    return p;
                }
            }

            return nullptr;
        }

        static wchar_t* Fill(wchar_t* dest, wchar_t val, size_t length)
        {
            return wmemset(dest, val, length);
        }

        static int IsAlnum(wchar_t ch) // NOLINT
        {
            return iswalnum(ch);
        }

        static int IsAlpha(wchar_t ch)
        {
            return iswalpha(ch);
        }

        static int IsLower(wchar_t ch)
        {
            return iswlower(ch);
        }

        static int IsUpper(wchar_t ch)
        {
            return iswupper(ch);
        }

        static int IsDigit(wchar_t ch)
        {
            return iswdigit(ch);
        }

        static int IsXDigit(wchar_t ch)
        {
            return iswxdigit(ch);
        }

        static int IsCntrl(wchar_t ch) // NOLINT
        {
            return iswcntrl(ch);
        }

        static int IsGraph(wchar_t ch)
        {
            return iswgraph(ch);
        }

        static int IsPrint(wchar_t ch)
        {
            return iswprint(ch);
        }

        static int IsPunct(wchar_t ch) // NOLINT
        {
            return iswpunct(ch);
        }

        static int IsSpace(wchar_t ch)
        {
            return iswspace(ch);
        }

        static wchar_t ToLower(wchar_t ch)
        {
            return towlower(ch);
        }

        static wchar_t ToUpper(wchar_t ch)
        {
            return towupper(ch);
        }

        constexpr static wchar_t GetSpace()
        {
            return L' ';
        }

        constexpr static wchar_t GetZero()
        {
            return L'0';
        }

        constexpr static wchar_t GetEndFlag()
        {
            return 0;
        }
    };

    namespace Shims
    {
        inline FL_CONSTEXPR11 const wchar_t* PtrOf(const wchar_t* rawString)
        {
            return rawString;
        }

        inline FL_CONSTEXPR11 const char* PtrOf(const char* rawString)
        {
            return rawString;
        }

        inline FL_CONSTEXPR17 size_t LengthOf(const wchar_t* rawString)
        {
            return std::char_traits<wchar_t>::length(rawString);
        }

        inline FL_CONSTEXPR17 size_t LengthOf(const char* rawString)
        {
            return std::char_traits<char>::length(rawString);
        }

        inline FL_CONSTEXPR17 const char* PtrOf(const std::basic_string<char>& str)
        {
            return str.c_str();
        }

        inline FL_CONSTEXPR17 const wchar_t* PtrOf(const std::basic_string<wchar_t>& str)
        {
            return str.c_str();
        }

        inline FL_CONSTEXPR17 size_t LengthOf(const std::basic_string<char>& str)
        {
            return str.length();
        }

        inline FL_CONSTEXPR17 size_t LengthOf(const std::basic_string<wchar_t>& str)
        {
            return str.length();
        }

#if FL_COMPILER_IS_GREATER_THAN_CXX11
        inline FL_CONSTEXPR11 const char16_t* PtrOf(const char16_t* rawString)
        {
            return rawString;
        }

        inline FL_CONSTEXPR17 size_t LengthOf(const char16_t* rawString)
        {
            return std::char_traits<char16_t>::length(rawString);
        }

        inline FL_CONSTEXPR17 const char16_t* PtrOf(const std::basic_string<char16_t>& str)
        {
            return str.c_str();
        }

        inline FL_CONSTEXPR17 size_t LengthOf(const std::basic_string<char16_t>& str)
        {
            return str.size();
        }

        inline FL_CONSTEXPR11 const char32_t* PtrOf(const char32_t* rawString)
        {
            return rawString;
        }

        inline FL_CONSTEXPR17 size_t LengthOf(const char32_t* rawString)
        {
            return std::char_traits<char32_t>::length(rawString);
        }

        inline FL_CONSTEXPR17 const char32_t* PtrOf(const std::basic_string<char32_t>& str)
        {
            return str.c_str();
        }

        inline FL_CONSTEXPR17 size_t LengthOf(const std::basic_string<char32_t>& str)
        {
            return str.size();
        }
#endif
    }
}
