#include <celero/Celero.h>
#include <Format/StandardLibraryAdapter.hpp>


void exampleFunction() {
    for (int i = 0; i < 1000; ++i) {
        
    }
}

CELERO_MAIN

BASELINE(ExampleBenchmark, Baseline, 10, 100) {
    exampleFunction();
}

BENCHMARK(ExampleBenchmark, ExampleFunction, 10, 100) {
    exampleFunction();
}