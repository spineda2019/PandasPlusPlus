#include <iostream>
#include <ostream>
#include <ppp/Matrix.hpp>
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
};

int main(void) {
    TestHeadlessPrint();
    TestEmptyPrint();
}
