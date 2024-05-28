#ifndef CSVLIBRARY_CINTERFACE_H
#define CSVLIBRARY_CINTERFACE_H

#include <stddef.h>
#include <stdint.h>

#include "common_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FColumn_t {
    void *opaque_ptr;
} *FColumn;

typedef struct DColumn_t {
    void *opaque_ptr;
} *DColumn;

typedef struct IColumn_t {
    void *opaque_ptr;
} *IColumn;

typedef struct LColumn_t {
    void *opaque_ptr;
} *LColumn;

/* **************************** Column Factories **************************** */

C_API FColumn NewFColumn(const float *data, size_t length, const char *key);

C_API DColumn NewDColumn(const double *data, size_t length, const char *key);

C_API IColumn NewIColumn(const int *data, size_t length, const char *key);

C_API LColumn NewLColumn(const long *data, size_t length, const char *key);

/* ****************************** Print Column ****************************** */

C_API void PrintFColumn(const FColumn column);

C_API void PrintDColumn(const DColumn column);

C_API void PrintIColumn(const IColumn column);

C_API void PrintLColumn(const LColumn column);

/* ******************************* Add Column ******************************* */

C_API FColumn AddFColumns(const FColumn lhs, const FColumn rhs);

C_API DColumn AddDColumns(const DColumn lhs, const DColumn rhs);

C_API IColumn AddIColumns(const IColumn lhs, const IColumn rhs);

C_API LColumn AddLColumns(const LColumn lhs, const LColumn rhs);

/* ***************************** Delete Column ****************************** */

C_API void DeleteFColumn(FColumn column);

C_API void DeleteDColumn(DColumn column);

C_API void DeleteIColumn(IColumn column);

C_API void DeleteLColumn(LColumn column);

/* ******************************* Sum Method ******************************* */

C_API float SumFColumn(const FColumn column);

C_API double SumDColumn(const DColumn column);

C_API int SumIColumn(const IColumn column);

C_API long SumLColumn(const LColumn column);

/* ****************************** Size Method ******************************* */

C_API size_t FColumnSize(const FColumn column);

C_API size_t DColumnSize(const DColumn column);

C_API size_t IColumnSize(const IColumn column);

C_API size_t LColumnSize(const LColumn column);

#ifdef __cplusplus
}
#endif

#endif  // CSVLIBRARY_CINTERFACE_H
