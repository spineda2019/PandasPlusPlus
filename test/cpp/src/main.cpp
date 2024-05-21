#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <ppp/CsvFile.hpp>
#include <ppp/Matrix.hpp>

#ifdef BENCHMARK
#include "include/benchmark.hpp"
#endif  // BENCHMARK

#include "include/column_tests.hpp"
#include "include/matrix_tests.hpp"

bool TestCsvConstruction() {
    const char* bar = "";
    (void)ppp::CsvFile::New("");
    return true;
}

int main(void) {
    std::unique_ptr<std::size_t> passes{std::make_unique<std::size_t>(0)};
    std::unique_ptr<std::size_t> fails{std::make_unique<std::size_t>(0)};

    bool test_result{matrix_test::MatrixMasterTest(passes, fails) &&
                     column_test::ColumnMasterTest(passes, fails)};

#ifdef BENCHMARK
    std::cout << "Benchmarking matrix operations..." << std::endl;
    benchmark::BenchMarkOperations();
#endif  // BENCHMARK

    std::cout << std::endl
              << "--------------------------------- Test Summary "
                 "---------------------------------"
              << std::endl
              << std::endl
              << "Tests Passed: " << *passes << std::endl
              << "Tests Failed: " << *fails << std::endl
              << std::endl
              << "------------------------------------ Result "
                 "------------------------------------"
              << std::endl
              << std::endl;
    if (test_result) {
        std::cout << "                               " << "All tests passed!!"
                  << "                               " << std::endl;
    } else {
        std::cout << "                              " << "Some tests failed..."
                  << "                              " << std::endl;
    }
}
