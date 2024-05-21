#include "include/CInterface.h"

#include <stdint.h>

#include <cstdlib>
#include <iostream>
#include <ppp/Column.hpp>
#include <ppp/Dataframe.hpp>

// Dataframe *create_dataframe_float(const char *file_path, bool
// file_has_header) {
//     return reinterpret_cast<Dataframe *>(
//         new ppp::Dataframe<float>(file_path, file_has_header));
// }

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
