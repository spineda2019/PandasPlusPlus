#include <complex>
#include <iostream>
#include <ostream>
#include <ppp/Matrix.hpp>
#include <span>
#include <string_view>
#include <vector>

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
        return true;
    } else {
        std::cout << "Test: TestHeadlessPrint Failed..." << std::endl
                  << std::endl;
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
        return true;
    } else {
        std::cout << "Test: TestEmptyPrint Failed..." << std::endl << std::endl;
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
        return true;
    } else {
        std::cout << "Test: TestComplexPrint Failed..." << std::endl
                  << std::endl;
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
        return true;
    } else {
        std::cout << "Test: TestInsertingHeaders Failed..." << std::endl
                  << std::endl;
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
        return false;
    } else {
        std::cout << "Test: TestBadShapeCatching Passed!" << std::endl
                  << std::endl;
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
        return false;
    } else {
        auto sum = lhs.value() + rhs.value();
        if (sum.has_value()) {
            std::cout << "Test: TestAddition Succeeded" << std::endl
                      << std::endl;
            std::cout << sum.value() << std::endl;
            return true;
        } else {
            return false;
        }
    }
}

int main(void) {
    TestHeadlessPrint();
    TestEmptyPrint();
    TestComplexPrint();
    TestInsertingHeaders();
    TestBadShapeCatching();
    TestAddition();
}
