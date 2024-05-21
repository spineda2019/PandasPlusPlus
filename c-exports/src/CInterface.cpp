#include "include/CInterface.h"

#include <stdint.h>

#include <cstdlib>
#include <iostream>
#include <ppp/Column.hpp>
#include <ppp/Dataframe.hpp>

#define EXTRACT_OBJECT(obj, type) *(reinterpret_cast<ppp::Column<type> *>(obj))

C_API FColumnHandle NewFColumn(const float *data, size_t length,
                               const char *key) {
    ppp::Column<float> *col{
        new ppp::Column{std::vector<float>{data, data + length}, key}};

    return col;
}

C_API void PrintFColumn(FColumnHandle column) {
    if (column) {
        std::cout << *(reinterpret_cast<ppp::Column<float> *>(column))
                  << std::endl;
    } else {
        std::cout << "Bad Column Handle..." << std::endl;
    }
}

C_API FColumnHandle AddFColumns(const FColumnHandle lhs,
                                const FColumnHandle rhs) {
    if (!lhs || !rhs) {
        return nullptr;
    }
    auto sum = EXTRACT_OBJECT(lhs, float) + EXTRACT_OBJECT(rhs, float);
    if (sum.has_value()) {
        return new ppp::Column<float>{std::move(sum.value())};
    } else {
        return nullptr;
    }
}

C_API void DeleteFColumn(const FColumnHandle column) {
    if (column) {
        delete reinterpret_cast<ppp::Column<float> *>(column);
    }
}
