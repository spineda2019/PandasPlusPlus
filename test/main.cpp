#include <complex>
#include <iostream>
#include <ostream>
#include <ppp/Matrix.hpp>
#include <span>
#include <string_view>
#include <vector>

void TestHeadlessPrint() {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    ppp::Matrix lvalue{data};

    data = {};

    std::cout << lvalue << std::endl;
}

void TestEmptyPrint() {
    ppp::Matrix<float> empty{};

    std::cout << empty << std::endl;
}

void TestComplexPrint() {
    std::vector<std::vector<std::complex<float>>> complex{{
        {std::complex<float>(3.0, 3.0), std::complex<float>(4.0, 4.0)},
        {std::complex<float>(5.0, 5.0), std::complex<float>(6.0, 6.0)},
    }};

    ppp::Matrix<std::complex<float>> cmp{complex};

    std::cout << cmp << std::endl;
}

bool TestInsertingHeaders() {
    std::vector<std::vector<float>> data{
        {3.0, 4.0},
        {3.0, 4.0},
    };

    ppp::Matrix lvalue{data};
    std::vector<std::string_view> headers{"Foo", "Bar"};
    if (const std::optional<std::uint8_t> result{lvalue.SetHeaders(headers)};
        result.has_value()) {
        std::cout << lvalue << std::endl;
        return true;
    } else {
        return false;
    }
}

int main(void) {
    TestHeadlessPrint();
    TestEmptyPrint();
    TestComplexPrint();
    TestInsertingHeaders();
}
