#include "include/ppp_column.h"

#include <stdint.h>

#include <cstdlib>
#include <iostream>
#include <ppp/Column.hpp>
#include <ppp/Dataframe.hpp>

#define EXTRACT_OBJECT(obj, type) *(reinterpret_cast<ppp::Column<type> *>(obj))

/* **************************** Column Factories **************************** */

#define NewColumnImplementation(type)                                  \
    ppp::Column<type> *col{                                            \
        new ppp::Column{std::vector<type>{data, data + length}, key}}; \
                                                                       \
    return col;

C_API FColumnHandle NewFColumn(const float *data, size_t length,
                               const char *key) {
    NewColumnImplementation(float);
}

C_API DColumnHandle NewDColumn(const double *data, size_t length,
                               const char *key) {
    NewColumnImplementation(double);
}

C_API IColumnHandle NewIColumn(const int *data, size_t length,
                               const char *key) {
    NewColumnImplementation(int);
}

C_API LColumnHandle NewLColumn(const long *data, size_t length,
                               const char *key) {
    NewColumnImplementation(long);
}

/* ****************************** Print Column ****************************** */

#define PrintColumnImplementation(type)                               \
    if (column) {                                                     \
        std::cout << *(reinterpret_cast<ppp::Column<type> *>(column)) \
                  << std::endl;                                       \
    } else {                                                          \
        std::cout << "Bad Column Handle..." << std::endl;             \
    }

C_API void PrintFColumn(const FColumnHandle column) {
    PrintColumnImplementation(float);
}

C_API void PrintDColumn(const DColumnHandle column) {
    PrintColumnImplementation(double);
}

C_API void PrintIColumn(const IColumnHandle column) {
    PrintColumnImplementation(int);
}

C_API void PrintLColumn(const LColumnHandle column) {
    PrintColumnImplementation(long);
}

/* ******************************* Add Column ******************************* */

#define AddColumnImplementation(type)                                   \
    if (!lhs || !rhs) {                                                 \
        return nullptr;                                                 \
    }                                                                   \
    auto sum = EXTRACT_OBJECT(lhs, float) + EXTRACT_OBJECT(rhs, float); \
    if (sum.has_value()) {                                              \
        return new ppp::Column<float>{std::move(sum.value())};          \
    } else {                                                            \
        return nullptr;                                                 \
    }

C_API FColumnHandle AddFColumns(const FColumnHandle lhs,
                                const FColumnHandle rhs) {
    AddColumnImplementation(float);
}

C_API DColumnHandle AddDColumns(const DColumnHandle lhs,
                                const DColumnHandle rhs) {
    AddColumnImplementation(double);
}

C_API IColumnHandle AddIColumns(const IColumnHandle lhs,
                                const IColumnHandle rhs) {
    AddColumnImplementation(int);
}

C_API LColumnHandle AddLColumns(const LColumnHandle lhs,
                                const LColumnHandle rhs) {
    AddColumnImplementation(long);
}

/* ***************************** Delete Column ****************************** */

#define DeleteColumnImplementation(type)                      \
    if (column) {                                             \
        delete reinterpret_cast<ppp::Column<type> *>(column); \
    }

C_API void DeleteFColumn(const FColumnHandle column) {
    DeleteColumnImplementation(float);
}

C_API void DeleteDColumn(const DColumnHandle column) {
    DeleteColumnImplementation(double);
}
C_API void DeleteIColumn(const IColumnHandle column) {
    DeleteColumnImplementation(int);
}

C_API void DeleteLColumn(const LColumnHandle column) {
    DeleteColumnImplementation(long);
}
