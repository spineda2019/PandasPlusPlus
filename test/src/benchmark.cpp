#include "include/benchmark.hpp"

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <vector>

#include "ppp/Matrix.hpp"

namespace benchmark {

auto time_operation = [](const std::function<void()>& operation) {
    auto start = std::chrono::high_resolution_clock::now();
    operation();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    return duration.count();
};

void BenchMarkOperations() {
    std::vector<std::vector<float>> data{
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 0.0f},
    };

    std::optional<ppp::Matrix<float>> lhs{ppp::Matrix<float>::New(data)};
    std::optional<ppp::Matrix<float>> rhs{ppp::Matrix<float>::New(data)};

    if (lhs.has_value() && rhs.has_value()) {
        std::cout << "Benchmarking addition..." << std::endl;
        constexpr std::uint64_t test_iters{1'000'000};
        std::uint64_t time =
            time_operation([&lhs, &rhs]() {
                for (std::size_t test{0}; test < test_iters; test++) {
                    lhs.value() + rhs.value();
                }
            }) /
            test_iters;
        std::cout << "Average 10x10 addition: " << time << "us" << std::endl;

        std::cout << "Benchmarking subtraction..." << std::endl;
        time = time_operation([&lhs, &rhs]() {
                   for (std::size_t test{0}; test < test_iters; test++) {
                       lhs.value() - rhs.value();
                   }
               }) /
               test_iters;
        std::cout << "Average 10x10 subtraction: " << time << "us" << std::endl;
    } else {
        std::cout << "Benchmarking skipped, unexpected result encountered.."
                  << std::endl;
    }
}
}  // namespace benchmark
