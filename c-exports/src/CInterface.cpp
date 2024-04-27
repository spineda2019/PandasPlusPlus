#include "CInterface.h"

#include <stdint.h>

#include <iostream>
#include <ppp/Dataframe.hpp>

// Dataframe *create_dataframe_float(const char *file_path, bool
// file_has_header) {
//     return reinterpret_cast<Dataframe *>(
//         new ppp::Dataframe<float>(file_path, file_has_header));
// }

void delete_Dataframe_float(Dataframe *frame) {
    delete reinterpret_cast<ppp::Dataframe<float> *>(frame);
}

void print_Dataframe_tail_float(Dataframe *frame, uint64_t n_rows) {
    reinterpret_cast<ppp::Dataframe<float> *>(frame)->PrintTail(n_rows);
}

void print_Dataframe_head_float(Dataframe *frame, uint64_t n_rows) {
    reinterpret_cast<ppp::Dataframe<float> *>(frame)->PrintData(n_rows);
}

float col_name_mean_float(Dataframe *frame, const char *col_name) {
    try {
        return reinterpret_cast<ppp::Dataframe<float> *>(frame)->Mean(col_name);
    } catch (ColumnNotFoundException &e) {
        std::cout << e.what() << std::endl;
        return 0.0f;
    }
}
