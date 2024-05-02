#include <chrono>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <ostream>
#include <ppp/CsvFile.hpp>
#include <ppp/Matrix.hpp>
#include <span>
#include <string_view>
#include <vector>

std::size_t passes{0};
std::size_t fails{0};

auto time_operation = [](const std::function<void()>& operation) {
    auto start = std::chrono::high_resolution_clock::now();
    operation();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    return duration.count();
};

bool TestEquality() {
    std::optional<ppp::Matrix<int>> empty_left{ppp::Matrix<int>::New()};
    std::optional<ppp::Matrix<int>> empty_right{ppp::Matrix<int>::New()};

    if ((!empty_left.has_value()) || (!empty_right.has_value())) {
        std::cout << "Test: TestEquality Failed..." << std::endl << std::endl;
        fails++;
        return false;
    } else if (empty_left.value() != empty_right.value()) {
        std::cout << "Test: TestEquality Failed..." << std::endl << std::endl;
        fails++;
        return false;
    } else {
        std::vector<std::vector<std::uint16_t>> some{{
            {1, 2, 3},
            {1, 2, 3},
            {1, 2, 3},
        }};

        std::optional<ppp::Matrix<std::uint16_t>> left{
            ppp::Matrix<std::uint16_t>::New(some)};
        std::optional<ppp::Matrix<std::uint16_t>> right{
            ppp::Matrix<std::uint16_t>::New(std::move(some))};
        if ((!left.has_value()) || (!right.has_value())) {
            std::cout << "Test: TestEquality Failed..." << std::endl
                      << std::endl;
            fails++;
            return false;
        } else {
            bool result = left.value() == right.value();
            if (result) {
                std::cout << "Test: TestEquality Passed!" << std::endl
                          << "Matrix Used: " << std::endl
                          << std::endl;
                std::cout << left.value() << std::endl;
                passes++;
                return true;
            } else {
                return false;
            }
        }
    }
}

bool TestHeadlessPrint() {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    std::optional<ppp::Matrix<float>> lvalue{ppp::Matrix<float>::New(data)};

    if (lvalue.has_value()) {
        std::cout << "Test: TestHeadlessPrint Passed!" << std::endl
                  << "Matrix Used: " << std::endl
                  << std::endl;
        std::cout << lvalue.value() << std::endl;
        passes++;
        return true;
    } else {
        std::cout << "Test: TestHeadlessPrint Failed..." << std::endl
                  << std::endl;
        fails++;
        return false;
    }
}

bool TestEmptyPrint() {
    std::optional<ppp::Matrix<float>> empty{ppp::Matrix<float>::New()};

    if (empty.has_value()) {
        std::cout << "Test: TestEmptyPrint Passed!" << std::endl
                  << "Matrix Used: " << std::endl
                  << std::endl;
        std::cout << empty.value() << std::endl;
        passes++;
        return true;
    } else {
        std::cout << "Test: TestEmptyPrint Failed..." << std::endl << std::endl;
        fails++;
        return false;
    }
}

bool TestComplexPrint() {
    std::vector<std::vector<std::complex<float>>> complex{{
        {std::complex<float>(3.0, 3.0), std::complex<float>(4.0, 4.0)},
        {std::complex<float>(5.0, 5.0), std::complex<float>(6.0, 6.0)},
    }};

    std::optional<ppp::Matrix<std::complex<float>>> cmp{
        ppp::Matrix<std::complex<float>>::New(complex)};

    if (cmp.has_value()) {
        std::cout << "Test: TestComplexPrint Passed!" << std::endl
                  << "Matrix Used: " << std::endl
                  << std::endl;
        std::cout << cmp.value() << std::endl;
        passes++;
        return true;
    } else {
        std::cout << "Test: TestComplexPrint Failed..." << std::endl
                  << std::endl;
        fails++;
        return false;
    }
}

bool TestInsertingHeaders() {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    std::optional<ppp::Matrix<float>> lvalue{ppp::Matrix<float>::New(data)};
    if (!lvalue.has_value()) {
        std::cout << "Test: TestInsertingHeaders Failed..." << std::endl
                  << std::endl;
        fails++;
        return false;
    }

    std::vector<std::string_view> headers{"Foo", "Bar"};
    if (const std::optional<std::uint8_t> result{
            lvalue.value().SetHeaders(headers)};
        result.has_value()) {
        std::cout << "Test: TestInsertingHeaders Passed!" << std::endl
                  << "Matrix Used: " << std::endl
                  << std::endl;
        std::cout << lvalue.value() << std::endl;
        passes++;
        return true;
    } else {
        std::cout << "Test: TestInsertingHeaders Failed..." << std::endl
                  << std::endl;
        fails++;
        return false;
    }
}

bool TestBadShapeCatching() {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0},
    };

    std::optional<ppp::Matrix<float>> lvalue{ppp::Matrix<float>::New(data)};
    if (lvalue.has_value()) {
        std::cout << "Test: TestInsertingHeaders Failed..." << std::endl
                  << std::endl;
        fails++;
        return false;
    } else {
        std::cout << "Test: TestBadShapeCatching Passed!" << std::endl
                  << std::endl;
        passes++;
        return true;
    }
}

bool TestAddition() {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    std::optional<ppp::Matrix<float>> lhs{ppp::Matrix<float>::New(data)};
    std::optional<ppp::Matrix<float>> rhs{ppp::Matrix<float>::New(data)};
    if (!lhs.has_value() || !rhs.has_value()) {
        std::cout << "Test: TestAddition Failed..." << std::endl << std::endl;
        fails++;
        return false;
    } else {
        auto sum = lhs.value() + rhs.value();
        if (sum.has_value()) {
            std::cout << "Test: TestAddition Passed!" << std::endl << std::endl;
            std::cout << sum.value() << std::endl;
            passes++;
            return true;
        } else {
            fails++;
            return false;
        }
    }
}

bool TestSubtraction() {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    std::optional<ppp::Matrix<float>> lhs{ppp::Matrix<float>::New(data)};
    std::optional<ppp::Matrix<float>> rhs{ppp::Matrix<float>::New(data)};
    if (!lhs.has_value() || !rhs.has_value()) {
        std::cout << "Test: TestSubtraction Failed..." << std::endl
                  << std::endl;
        fails++;
        return false;
    } else {
        auto sum = lhs.value() - rhs.value();
        if (sum.has_value()) {
            std::cout << "Test: TestSubtraction Passed!" << std::endl
                      << std::endl;
            std::cout << sum.value() << std::endl;
            passes++;
            return true;
        } else {
            fails++;
            return false;
        }
    }
}

bool TestNonMatrixSubtraction() {
    std::vector<std::vector<double>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    const std::optional<ppp::Matrix<double>> lhs{
        ppp::Matrix<double>::New(data)};

    if (lhs.has_value()) {
        const std::optional<ppp::Matrix<double>> diff{lhs.value() - 4.0};
        if (diff.has_value()) {
            passes++;
            std::cout << "Test: TestNonMatrixSubtraction Passed!" << std::endl
                      << std::endl;
            std::cout << diff.value() << std::endl;
            return true;
        } else {
            fails++;
            std::cout << "Test: TestNonMatrixSubtraction Failed..." << std::endl
                      << std::endl;
            return false;
        }
    } else {
        fails++;
        std::cout << "Test: TestNonMatrixSubtraction Failed..." << std::endl
                  << std::endl;
        return false;
    }
}

bool TestCsvConstruction() {
    const char* bar = "";
    (void)ppp::CsvFile::New("");
    return true;
}

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

int main(void) {
    bool test_result{TestEquality() && TestHeadlessPrint() &&
                     TestEmptyPrint() && TestComplexPrint() &&
                     TestInsertingHeaders() && TestBadShapeCatching() &&
                     TestAddition() && TestSubtraction() &&
                     TestNonMatrixSubtraction() && TestCsvConstruction()};

    std::cout << "Benchmarking matrix operations..." << std::endl;
    BenchMarkOperations();

    std::cout << std::endl
              << "--------------------------------- Test Summary "
                 "---------------------------------"
              << std::endl
              << std::endl
              << "Tests Passed: " << passes << std::endl
              << "Tests Failed: " << fails << std::endl
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
