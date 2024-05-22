#include "include/ppp_column.h"

#include <stdint.h>

#include <cstdlib>
#include <iostream>
#include <ppp/Column.hpp>
#include <ppp/Dataframe.hpp>

#define EXTRACT_OBJECT(obj, type) *(reinterpret_cast<ppp::Column<type> *>(obj))

/* **************************** Column Factories **************************** */

#define NewColumnImplementation(type, struct_ptr)                      \
    ppp::Column<type> *col{                                            \
        new ppp::Column{std::vector<type>{data, data + length}, key}}; \
    struct_ptr->opaque_ptr = col;                                      \
                                                                       \
    return opaque_struct;

C_API FColumn NewFColumn(const float *data, size_t length, const char *key) {
    FColumn opaque_struct = new FColumn_t{};
    NewColumnImplementation(float, opaque_struct);
}

C_API DColumn NewDColumn(const double *data, size_t length, const char *key) {
    DColumn opaque_struct = new DColumn_t{};
    NewColumnImplementation(double, opaque_struct);
}

C_API IColumn NewIColumn(const int *data, size_t length, const char *key) {
    IColumn opaque_struct = new IColumn_t{};
    NewColumnImplementation(int, opaque_struct);
}

C_API LColumn NewLColumn(const long *data, size_t length, const char *key) {
    LColumn opaque_struct = new LColumn_t{};
    NewColumnImplementation(long, opaque_struct);
}

/* ****************************** Print Column ****************************** */

#define PrintColumnImplementation(type)                       \
    if (column && column->opaque_ptr) {                       \
        std::cout << *(reinterpret_cast<ppp::Column<type> *>( \
                         column->opaque_ptr))                 \
                  << std::endl;                               \
    } else {                                                  \
        std::cout << "Bad Column Handle..." << std::endl;     \
    }

C_API void PrintFColumn(const FColumn column) {
    PrintColumnImplementation(float);
}

C_API void PrintDColumn(const DColumn column) {
    PrintColumnImplementation(double);
}

C_API void PrintIColumn(const IColumn column) {
    PrintColumnImplementation(int);
}

C_API void PrintLColumn(const LColumn column) {
    PrintColumnImplementation(long);
}

/* ******************************* Add Column ******************************* */

#define AddColumnImplementation(type, opaque_struct)                         \
    if (!lhs || !rhs) {                                                      \
        return nullptr;                                                      \
    } else if (!lhs->opaque_ptr || !rhs->opaque_ptr) {                       \
        return nullptr;                                                      \
    }                                                                        \
    auto sum = (*(reinterpret_cast<ppp::Column<type> *>(lhs->opaque_ptr))) + \
               (*(reinterpret_cast<ppp::Column<type> *>(rhs->opaque_ptr)));  \
    if (sum.has_value()) {                                                   \
        auto new_col = new ppp::Column<type>{std::move(sum.value())};        \
        opaque_struct->opaque_ptr = new_col;                                 \
        return opaque_struct;                                                \
    } else {                                                                 \
        return nullptr;                                                      \
    }

C_API FColumn AddFColumns(const FColumn lhs, const FColumn rhs) {
    FColumn opaque_struct = new FColumn_t{};
    AddColumnImplementation(float, opaque_struct);
}

C_API DColumn AddDColumns(const DColumn lhs, const DColumn rhs) {
    DColumn opaque_struct = new DColumn_t{};
    AddColumnImplementation(double, opaque_struct);
}

C_API IColumn AddIColumns(const IColumn lhs, const IColumn rhs) {
    IColumn opaque_struct = new IColumn_t{};
    AddColumnImplementation(int, opaque_struct);
}

C_API LColumn AddLColumns(const LColumn lhs, const LColumn rhs) {
    LColumn opaque_struct = new LColumn_t{};
    AddColumnImplementation(long, opaque_struct);
}

/* ***************************** Delete Column ****************************** */

#define DeleteColumnImplementation(type)                                      \
    if (column) {                                                             \
        if (column->opaque_ptr) {                                             \
            delete reinterpret_cast<ppp::Column<type> *>(column->opaque_ptr); \
            delete column;                                                    \
        } else {                                                              \
            delete column;                                                    \
        }                                                                     \
    }

C_API void DeleteFColumn(FColumn column) { DeleteColumnImplementation(float); }

C_API void DeleteDColumn(DColumn column) { DeleteColumnImplementation(double); }

C_API void DeleteIColumn(IColumn column) { DeleteColumnImplementation(int); }

C_API void DeleteLColumn(LColumn column) { DeleteColumnImplementation(long); }
