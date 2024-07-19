// ReSharper disable CppUseStdSize
#include <celero/Celero.h>
#include <Format/StandardLibraryAdapter.hpp>
#include <iostream>

#if FL_COMPILER_IS_GREATER_THAN_CXX20
#include <format>
#endif

char GSprintfBuffer[1024];

void test_c_sprintf()
{
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d %s %.2f %d %s %.2f", 123, "hello", 1.23, 456, "world", 4.56);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d - %s - %.2f - %d - %s - %.2f", 789, "foo", 7.89, 101, "bar", 10.11);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d + %s + %.2f + %d + %s + %.2f", 112, "baz", 11.12, 131, "qux", 13.14);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d * %s * %.2f * %d * %s * %.2f", 215, "alpha", 21.52, 314, "beta", 31.45);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d / %s / %.2f / %d / %s / %.2f", 516, "gamma", 51.67, 617, "delta", 61.78);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d ^ %s ^ %.2f ^ %d ^ %s ^ %.2f", 718, "epsilon", 71.89, 819, "zeta", 81.90);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d %% %s %% %.2f %% %d %% %s %% %.2f", 920, "eta", 92.01, 101, "theta", 10.12);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d & %s & %.2f & %d & %s & %.2f", 122, "iota", 12.23, 233, "kappa", 23.34);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d | %s | %.2f | %d | %s | %.2f", 344, "lambda", 34.45, 455, "mu", 45.56);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%d ~ %s ~ %.2f ~ %d ~ %s ~ %.2f", 566, "nu", 56.67, 677, "xi", 67.78);

    // Padding and alignment
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%5d", 123);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%-5d", 123);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%6d", -123);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%-6d", -123);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%s", "\n\t\"");
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%e", 123.456);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%E", 123.456);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%.3e", 123.456);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%.3E", 123.456);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%x", 123);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%X", 123);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%08x", 123);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%08X", 123);
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%5s", "hi");
    Formatting::TCharTraits<char>::StringPrintf(GSprintfBuffer, FL_ARRAY_COUNTOF(GSprintfBuffer), "%-5s", "hi");
}

#if FL_COMPILER_IS_GREATER_THAN_CXX20
std::string GStdFormatResult;
void test_std_format()
{
    GStdFormatResult = std::format("{} {} {:.2f} {} {} {:.2f}", 123, "hello", 1.23, 456, "world", 4.56);
    GStdFormatResult = std::format("{} - {} - {:.2f} - {} - {} - {:.2f}", 789, "foo", 7.89, 101, "bar", 10.11);
    GStdFormatResult = std::format("{} + {} + {:.2f} + {} + {} + {:.2f}", 112, "baz", 11.12, 131, "qux", 13.14);
    GStdFormatResult = std::format("{} * {} * {:.2f} * {} * {} * {:.2f}", 215, "alpha", 21.52, 314, "beta", 31.45);
    GStdFormatResult = std::format("{} / {} / {:.2f} / {} / {} / {:.2f}", 516, "gamma", 51.67, 617, "delta", 61.78);
    GStdFormatResult = std::format("{} ^ {} ^ {:.2f} ^ {} ^ {} ^ {:.2f}", 718, "epsilon", 71.89, 819, "zeta", 81.90);
    GStdFormatResult = std::format("{} % {} % {:.2f} % {} % {} % {:.2f}", 920, "eta", 92.01, 101, "theta", 10.12);
    GStdFormatResult = std::format("{} & {} & {:.2f} & {} & {} & {:.2f}", 122, "iota", 12.23, 233, "kappa", 23.34);
    GStdFormatResult = std::format("{} | {} | {:.2f} | {} | {} | {:.2f}", 344, "lambda", 34.45, 455, "mu", 45.56);
    GStdFormatResult = std::format("{} ~ {} ~ {:.2f} ~ {} ~ {} ~ {:.2f}", 566, "nu", 56.67, 677, "xi", 67.78);

    // Padding and alignment
    GStdFormatResult = std::format("{:5}", 123);
    GStdFormatResult = std::format("{:-5}", 123);
    GStdFormatResult = std::format("{:6}", -123);
    GStdFormatResult = std::format("{:-6}", -123);
    GStdFormatResult = std::format("{}", "\n\t\"");
    GStdFormatResult = std::format("{:e}", 123.456);
    GStdFormatResult = std::format("{:E}", 123.456);
    GStdFormatResult = std::format("{:.3e}", 123.456);
    GStdFormatResult = std::format("{:.3E}", 123.456);
    GStdFormatResult = std::format("{:x}", 123);
    GStdFormatResult = std::format("{:X}", 123);
    GStdFormatResult = std::format("{:08x}", 123);
    GStdFormatResult = std::format("{:08X}", 123);
    GStdFormatResult = std::format("{}", "hi");
    GStdFormatResult = std::format("{}", "hi");
}
#endif

std::string GCppFormatLibraryCommonResult;
void test_cpp_format_library_common()
{
    using namespace Formatting;
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} {1} {2} {3} {4} {5}", 123, "hello", 1.23, 456, "world", 4.56);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} - {1} - {2} - {3} - {4} - {5}", 789, "foo", 7.89, 101, "bar", 10.11);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} + {1} + {2} + {3} + {4} + {5}", 112, "baz", 11.12, 131, "qux", 13.14);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} * {1} * {2} * {3} * {4} * {5}", 215, "alpha", 21.52, 314, "beta", 31.45);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} / {1} / {2} / {3} / {4} / {5}", 516, "gamma", 51.67, 617, "delta", 61.78);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} ^ {1} ^ {2} ^ {3} ^ {4} ^ {5}", 718, "epsilon", 71.89, 819, "zeta", 81.90);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} % {1} % {2} % {3} % {4} % {5}", 920, "eta", 92.01, 101, "theta", 10.12);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} & {1} & {2} & {3} & {4} & {5}", 122, "iota", 12.23, 233, "kappa", 23.34);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} | {1} | {2} | {3} | {4} | {5}", 344, "lambda", 34.45, 455, "mu", 45.56);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0} ~ {1} ~ {2} ~ {3} ~ {4} ~ {5}", 566, "nu", 56.67, 677, "xi", 67.78);

    // Padding and alignment
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0,5}", 123);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0,-5}", 123);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0,6}", -123);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0,-6}", -123);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0}", "\n\t\"");
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0:e}", 123.456);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0:E}", 123.456);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0:e3}", 123.456);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0:E3}", 123.456);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0:x}", 123);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0:X}", 123);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0:x8}", 123);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0:X8}", 123);
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0,5}", "hi");
    StandardLibrary::FormatTo(GCppFormatLibraryCommonResult, "{0,-5}", "hi");
}

std::string GCppFormatLibraryOptimizedResult;

void test_cpp_format_library_optimized()
{
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} {1} {2} {3} {4} {5}", 123, "hello", 1.23, 456, "world", 4.56);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} - {1} - {2} - {3} - {4} - {5}", 789, "foo", 7.89, 101, "bar", 10.11);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} + {1} + {2} + {3} + {4} + {5}", 112, "baz", 11.12, 131, "qux", 13.14);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} * {1} * {2} * {3} * {4} * {5}", 215, "alpha", 21.52, 314, "beta", 31.45);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} / {1} / {2} / {3} / {4} / {5}", 516, "gamma", 51.67, 617, "delta", 61.78);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} ^ {1} ^ {2} ^ {3} ^ {4} ^ {5}", 718, "epsilon", 71.89, 819, "zeta", 81.90);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} % {1} % {2} % {3} % {4} % {5}", 920, "eta", 92.01, 101, "theta", 10.12);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} & {1} & {2} & {3} & {4} & {5}", 122, "iota", 12.23, 233, "kappa", 23.34);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} | {1} | {2} | {3} | {4} | {5}", 344, "lambda", 34.45, 455, "mu", 45.56);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0} ~ {1} ~ {2} ~ {3} ~ {4} ~ {5}", 566, "nu", 56.67, 677, "xi", 67.78);

    // Padding and alignment
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0,5}", 123);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0,-5}", 123);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0,6}", -123);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0,-6}", -123);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0}", "\n\t\"");
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0:e}", 123.456);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0:E}", 123.456);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0:e3}", 123.456);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0:E3}", 123.456);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0:x}", 123);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0:X}", 123);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0:x8}", 123);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0:X8}", 123);
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0,5}", "hi");
    FL_STD_FORMAT_TO(GCppFormatLibraryOptimizedResult, "{0,-5}", "hi");
}

constexpr int SamplesCount = 10;

#if FL_DEBUG
constexpr int IterationsCount = 100;
#else
constexpr int IterationsCount = 10000;
#endif

BASELINE(CPPFormatLibraryBenchmark, Baseline, SamplesCount, IterationsCount)
{
    test_c_sprintf();    
}

#if FL_COMPILER_IS_GREATER_THAN_CXX20
BENCHMARK(CPPFormatLibraryBenchmark, StdFormat, SamplesCount, IterationsCount)
{
    test_std_format();
}
#endif


BENCHMARK(CPPFormatLibraryBenchmark, Common, SamplesCount, IterationsCount)
{
    test_cpp_format_library_common();
}

BENCHMARK(CPPFormatLibraryBenchmark, Optimized, SamplesCount, IterationsCount)
{
    test_cpp_format_library_optimized();
}

int main(int argc, char** argv)
{    
#if FL_COMPILER_IS_CXX23
    FL_CONSTEXPR11 const char* CppStandardVersion = "C++ 23";
#elif FL_COMPILER_IS_CXX20
    FL_CONSTEXPR11 const char* CppStandardVersion = "C++ 20";
#elif FL_COMPILER_IS_CXX17
    FL_CONSTEXPR11 const char* CppStandardVersion = "C++ 17";
#elif FL_COMPILER_IS_CXX14
    FL_CONSTEXPR11 auto CppStandardVersion = "C++ 14";
#elif FL_COMPILER_IS_CXX11
    FL_CONSTEXPR11 const char* CppStandardVersion = "C++ 11";
#elif FL_COMPILER_IS_CXX98
    FL_CONSTEXPR11 const char* CppStandardVersion = "C++ 98";
#else
    #error "can't find C++ compiler version."
#endif

    std::cout << "C++ Version:" << CppStandardVersion << std::endl;  // NOLINT(performance-avoid-endl)

    celero::Run(argc, argv);
}