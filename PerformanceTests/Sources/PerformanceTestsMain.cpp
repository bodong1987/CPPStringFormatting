// ReSharper disable CppClangTidyPerformanceInefficientVectorOperation
// ReSharper disable CppLocalVariableMayBeConst
#include <Format/StandardLibraryAdapter.hpp>

#if !FL_COMPILER_IS_GREATER_THAN_CXX11
#error "Need C++ 11"
#endif


#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <iomanip>

using namespace Formatting;
using Clock = std::chrono::high_resolution_clock;

void test_formatting_standard_library(int iterations) 
{
    std::string str;
    for (int i = 0; i < iterations; ++i) 
    {
        StandardLibrary::FormatTo(str, "{0} {1} {2} {3} {4} {5}", 123, "hello", 1.23, 456, "world", 4.56);
        StandardLibrary::FormatTo(str, "{0} - {1} - {2} - {3} - {4} - {5}", 789, "foo", 7.89, 101, "bar", 10.11);
        StandardLibrary::FormatTo(str, "{0} + {1} + {2} + {3} + {4} + {5}", 112, "baz", 11.12, 131, "qux", 13.14);
        StandardLibrary::FormatTo(str, "{0} * {1} * {2} * {3} * {4} * {5}", 215, "alpha", 21.52, 314, "beta", 31.45);
        StandardLibrary::FormatTo(str, "{0} / {1} / {2} / {3} / {4} / {5}", 516, "gamma", 51.67, 617, "delta", 61.78);
        StandardLibrary::FormatTo(str, "{0} ^ {1} ^ {2} ^ {3} ^ {4} ^ {5}", 718, "epsilon", 71.89, 819, "zeta", 81.90);
        StandardLibrary::FormatTo(str, "{0} % {1} % {2} % {3} % {4} % {5}", 920, "eta", 92.01, 101, "theta", 10.12);
        StandardLibrary::FormatTo(str, "{0} & {1} & {2} & {3} & {4} & {5}", 122, "iota", 12.23, 233, "kappa", 23.34);
        StandardLibrary::FormatTo(str, "{0} | {1} | {2} | {3} | {4} | {5}", 344, "lambda", 34.45, 455, "mu", 45.56);
        StandardLibrary::FormatTo(str, "{0} ~ {1} ~ {2} ~ {3} ~ {4} ~ {5}", 566, "nu", 56.67, 677, "xi", 67.78);

        // Padding and alignment
        StandardLibrary::FormatTo(str, "{0,5}", 123);
        StandardLibrary::FormatTo(str, "{0,-5}", 123);
        StandardLibrary::FormatTo(str, "{0,6}", -123);
        StandardLibrary::FormatTo(str, "{0,-6}", -123);
        StandardLibrary::FormatTo(str, "{0}", "\n\t\"");
        StandardLibrary::FormatTo(str, "{0:e}", 123.456);
        StandardLibrary::FormatTo(str, "{0:E}", 123.456);
        StandardLibrary::FormatTo(str, "{0:e3}", 123.456);
        StandardLibrary::FormatTo(str, "{0:E3}", 123.456);
        StandardLibrary::FormatTo(str, "{0:x}", 123);
        StandardLibrary::FormatTo(str, "{0:X}", 123);
        StandardLibrary::FormatTo(str, "{0:x8}", 123);
        StandardLibrary::FormatTo(str, "{0:X8}", 123);
        StandardLibrary::FormatTo(str, "{0,5}", "hi");
        StandardLibrary::FormatTo(str, "{0,-5}", "hi");
    }
}

void test_formatting_standard_library_macros(int iterations) 
{
    std::string str;
    for (int i = 0; i < iterations; ++i) 
    {
        FL_STD_FORMAT_TO(str, "{0} {1} {2} {3} {4} {5}", 123, "hello", 1.23, 456, "world", 4.56);
        FL_STD_FORMAT_TO(str, "{0} - {1} - {2} - {3} - {4} - {5}", 789, "foo", 7.89, 101, "bar", 10.11);
        FL_STD_FORMAT_TO(str, "{0} + {1} + {2} + {3} + {4} + {5}", 112, "baz", 11.12, 131, "qux", 13.14);
        FL_STD_FORMAT_TO(str, "{0} * {1} * {2} * {3} * {4} * {5}", 215, "alpha", 21.52, 314, "beta", 31.45);
        FL_STD_FORMAT_TO(str, "{0} / {1} / {2} / {3} / {4} / {5}", 516, "gamma", 51.67, 617, "delta", 61.78);
        FL_STD_FORMAT_TO(str, "{0} ^ {1} ^ {2} ^ {3} ^ {4} ^ {5}", 718, "epsilon", 71.89, 819, "zeta", 81.90);
        FL_STD_FORMAT_TO(str, "{0} % {1} % {2} % {3} % {4} % {5}", 920, "eta", 92.01, 101, "theta", 10.12);
        FL_STD_FORMAT_TO(str, "{0} & {1} & {2} & {3} & {4} & {5}", 122, "iota", 12.23, 233, "kappa", 23.34);
        FL_STD_FORMAT_TO(str, "{0} | {1} | {2} | {3} | {4} | {5}", 344, "lambda", 34.45, 455, "mu", 45.56);
        FL_STD_FORMAT_TO(str, "{0} ~ {1} ~ {2} ~ {3} ~ {4} ~ {5}", 566, "nu", 56.67, 677, "xi", 67.78);

        // Padding and alignment
        FL_STD_FORMAT_TO(str, "{0,5}", 123);
        FL_STD_FORMAT_TO(str, "{0,-5}", 123);
        FL_STD_FORMAT_TO(str, "{0,6}", -123);
        FL_STD_FORMAT_TO(str, "{0,-6}", -123);
        FL_STD_FORMAT_TO(str, "{0}", "\n\t\"");
        FL_STD_FORMAT_TO(str, "{0:e}", 123.456);
        FL_STD_FORMAT_TO(str, "{0:E}", 123.456);
        FL_STD_FORMAT_TO(str, "{0:e3}", 123.456);
        FL_STD_FORMAT_TO(str, "{0:E3}", 123.456);
        FL_STD_FORMAT_TO(str, "{0:x}", 123);
        FL_STD_FORMAT_TO(str, "{0:X}", 123);
        FL_STD_FORMAT_TO(str, "{0:x8}", 123);
        FL_STD_FORMAT_TO(str, "{0:X8}", 123);
        FL_STD_FORMAT_TO(str, "{0,5}", "hi");
        FL_STD_FORMAT_TO(str, "{0,-5}", "hi");
    }
}

void run_single_thread_tests(int iterations) 
{
    auto start = Clock::now();
    test_formatting_standard_library(iterations);
    auto end = Clock::now();
    double standard_library_time = std::chrono::duration<double>(end - start).count();

    start = Clock::now();
    test_formatting_standard_library_macros(iterations);
    end = Clock::now();
    double standard_library_macros_time = std::chrono::duration<double>(end - start).count();

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "|           | " << standard_library_time << " | " << standard_library_macros_time << " |\n";
}

void run_multi_thread_tests(int iterations, int thread_count) 
{
    auto run_test = [iterations](auto test_function) 
    {
        for (int i = 0; i < iterations; ++i) 
        {
            test_function(1);
        }
    };

    std::vector<std::thread> threads;

    // StandardLibrary::Format
    auto start = Clock::now();
    for (int i = 0; i < thread_count; ++i) 
    {
        threads.emplace_back(run_test, test_formatting_standard_library);
    }
    for (auto& thread : threads) 
    {
        thread.join();
    }
    auto end = Clock::now();
    double standard_library_time = std::chrono::duration<double>(end - start).count();
    threads.clear();

    // StandardLibrary::Format macros
    start = Clock::now();
    for (int i = 0; i < thread_count; ++i) 
    {
        threads.emplace_back(run_test, test_formatting_standard_library_macros);
    }
    for (auto& thread : threads) 
    {
        thread.join();
    }
    end = Clock::now();
    double standard_library_macros_time = std::chrono::duration<double>(end - start).count();
    threads.clear();

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "|           | " << standard_library_time << " | " << standard_library_macros_time << " |\n";
}

int main() 
{
    FL_CONSTEXPR11 const int iterations = 500000;
    FL_CONSTEXPR11 const int thread_count = 4;

    std::cout << "Single-threaded tests:\n";
    std::cout << "| Test Type | StandardLibrary::FormatTo (s) | StandardLibrary::FormatTo Macros (s) |\n";
    std::cout << "|-----------|-------------------------------|--------------------------------------|\n";
    run_single_thread_tests(iterations);

    std::cout << "\nMulti-threaded(" << thread_count << " threads) tests:\n";
    std::cout << "| Test Type | StandardLibrary::FormatTo (s) | StandardLibrary::FormatTo Macros (s) |\n";
    std::cout << "|-----------|-------------------------------|--------------------------------------|\n";
    run_multi_thread_tests(iterations, thread_count);
}