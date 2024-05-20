#include "include/matrix_tests.hpp"

#include <complex>
#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <string_view>

#include "ppp/Matrix.hpp"

namespace matrix_test {
namespace {
bool TestEquality(const std::unique_ptr<std::size_t>& passes,
                  const std::unique_ptr<std::size_t>& fails) {
    std::optional<ppp::Matrix<int>> empty_left{ppp::Matrix<int>::New()};
    std::optional<ppp::Matrix<int>> empty_right{ppp::Matrix<int>::New()};

    if ((!empty_left.has_value()) || (!empty_right.has_value())) {
        std::cout << "Test: TestEquality Failed..." << std::endl << std::endl;
        (*fails)++;
        return false;
    } else if (empty_left.value() != empty_right.value()) {
        std::cout << "Test: TestEquality Failed..." << std::endl << std::endl;
        (*fails)++;
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
            (*fails)++;
            return false;
        } else {
            bool result = left.value() == right.value();
            if (result) {
                std::cout << "Test: TestEquality Passed!" << std::endl
                          << "Matrix Used: " << std::endl
                          << std::endl;
                std::cout << left.value() << std::endl;
                (*passes)++;
                return true;
            } else {
                return false;
            }
        }
    }
}

bool TestHeadlessPrint(const std::unique_ptr<std::size_t>& passes,
                       const std::unique_ptr<std::size_t>& fails) {
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
        (*passes)++;
        return true;
    } else {
        std::cout << "Test: TestHeadlessPrint Failed..." << std::endl
                  << std::endl;
        (*fails)++;
        return false;
    }
}

bool TestEmptyPrint(const std::unique_ptr<std::size_t>& passes,
                    const std::unique_ptr<std::size_t>& fails) {
    std::optional<ppp::Matrix<float>> empty{ppp::Matrix<float>::New()};

    if (empty.has_value()) {
        std::cout << "Test: TestEmptyPrint Passed!" << std::endl
                  << "Matrix Used: " << std::endl
                  << std::endl;
        std::cout << empty.value() << std::endl;
        (*passes)++;
        return true;
    } else {
        std::cout << "Test: TestEmptyPrint Failed..." << std::endl << std::endl;
        (*fails)++;
        return false;
    }
}

bool TestComplexPrint(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
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
        (*passes)++;
        return true;
    } else {
        std::cout << "Test: TestComplexPrint Failed..." << std::endl
                  << std::endl;
        (*fails)++;
        return false;
    }
}

bool TestInsertingHeaders(const std::unique_ptr<std::size_t>& passes,
                          const std::unique_ptr<std::size_t>& fails) {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    std::optional<ppp::Matrix<float>> lvalue{ppp::Matrix<float>::New(data)};
    if (!lvalue.has_value()) {
        std::cout << "Test: TestInsertingHeaders Failed..." << std::endl
                  << std::endl;
        (*fails)++;
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
        (*passes)++;
        return true;
    } else {
        std::cout << "Test: TestInsertingHeaders Failed..." << std::endl
                  << std::endl;
        (*fails)++;
        return false;
    }
}

bool TestBadShapeCatching(const std::unique_ptr<std::size_t>& passes,
                          const std::unique_ptr<std::size_t>& fails) {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0},
    };

    std::optional<ppp::Matrix<float>> lvalue{ppp::Matrix<float>::New(data)};
    if (lvalue.has_value()) {
        std::cout << "Test: TestInsertingHeaders Failed..." << std::endl
                  << std::endl;
        (*fails)++;
        return false;
    } else {
        std::cout << "Test: TestBadShapeCatching Passed!" << std::endl
                  << std::endl;
        (*passes)++;
        return true;
    }
}

bool TestAddition(const std::unique_ptr<std::size_t>& passes,
                  const std::unique_ptr<std::size_t>& fails) {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    std::optional<ppp::Matrix<float>> lhs{ppp::Matrix<float>::New(data)};
    std::optional<ppp::Matrix<float>> rhs{ppp::Matrix<float>::New(data)};
    if (!lhs.has_value() || !rhs.has_value()) {
        std::cout << "Test: TestAddition Failed..." << std::endl << std::endl;
        (*fails)++;
        return false;
    } else {
        auto sum = lhs.value() + rhs.value();
        if (sum.has_value()) {
            std::cout << "Test: TestAddition Passed!" << std::endl << std::endl;
            std::cout << sum.value() << std::endl;
            (*passes)++;
            return true;
        } else {
            (*fails)++;
            return false;
        }
    }
}

bool TestSubtraction(const std::unique_ptr<std::size_t>& passes,
                     const std::unique_ptr<std::size_t>& fails) {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    std::optional<ppp::Matrix<float>> lhs{ppp::Matrix<float>::New(data)};
    std::optional<ppp::Matrix<float>> rhs{ppp::Matrix<float>::New(data)};
    if (!lhs.has_value() || !rhs.has_value()) {
        std::cout << "Test: TestSubtraction Failed..." << std::endl
                  << std::endl;
        (*fails)++;
        return false;
    } else {
        auto sum = lhs.value() - rhs.value();
        if (sum.has_value()) {
            std::cout << "Test: TestSubtraction Passed!" << std::endl
                      << std::endl;
            std::cout << sum.value() << std::endl;
            (*passes)++;
            return true;
        } else {
            (*fails)++;
            return false;
        }
    }
}

bool TestNonMatrixSubtraction(const std::unique_ptr<std::size_t>& passes,
                              const std::unique_ptr<std::size_t>& fails) {
    std::vector<std::vector<double>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    const std::optional<ppp::Matrix<double>> lhs{
        ppp::Matrix<double>::New(data)};

    if (lhs.has_value()) {
        const std::optional<ppp::Matrix<double>> diff{lhs.value() - 4.0};
        if (diff.has_value()) {
            (*passes)++;
            std::cout << "Test: TestNonMatrixSubtraction Passed!" << std::endl
                      << std::endl;
            std::cout << diff.value() << std::endl;
            return true;
        } else {
            (*fails)++;
            std::cout << "Test: TestNonMatrixSubtraction Failed..." << std::endl
                      << std::endl;
            return false;
        }
    } else {
        (*fails)++;
        std::cout << "Test: TestNonMatrixSubtraction Failed..." << std::endl
                  << std::endl;
        return false;
    }
}
}  // namespace

bool MatrixMasterTest(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
    return TestEquality(passes, fails) && TestHeadlessPrint(passes, fails) &&
           TestEmptyPrint(passes, fails) && TestComplexPrint(passes, fails) &&
           TestInsertingHeaders(passes, fails) &&
           TestBadShapeCatching(passes, fails) && TestAddition(passes, fails) &&
           TestSubtraction(passes, fails) &&
           TestNonMatrixSubtraction(passes, fails);
}

}  // namespace matrix_test
