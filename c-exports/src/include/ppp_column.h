#ifndef CSVLIBRARY_CINTERFACE_H
#define CSVLIBRARY_CINTERFACE_H

#include <stddef.h>
#include <stdint.h>

#include "common_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *FColumnHandle;
typedef void *DColumnHandle;
typedef void *IColumnHandle;
typedef void *LColumnHandle;

/* **************************** Column Factories **************************** */

C_API FColumnHandle NewFColumn(const float *data, size_t length,
                               const char *key);

C_API DColumnHandle NewDColumn(const double *data, size_t length,
                               const char *key);

C_API IColumnHandle NewIColumn(const int *data, size_t length, const char *key);

C_API LColumnHandle NewLColumn(const long *data, size_t length,
                               const char *key);

/* ****************************** Print Column ****************************** */

C_API void PrintFColumn(FColumnHandle column);

C_API void PrintDColumn(DColumnHandle column);

C_API void PrintIColumn(IColumnHandle column);

C_API void PrintLColumn(LColumnHandle column);

/* ******************************* Add Column ******************************* */

C_API FColumnHandle AddFColumns(const FColumnHandle lhs,
                                const FColumnHandle rhs);

C_API DColumnHandle AddDColumns(const DColumnHandle lhs,
                                const DColumnHandle rhs);

C_API IColumnHandle AddIColumns(const IColumnHandle lhs,
                                const IColumnHandle rhs);

C_API LColumnHandle AddLColumns(const LColumnHandle lhs,
                                const LColumnHandle rhs);

/* ***************************** Delete Column ****************************** */

C_API void DeleteFColumn(const FColumnHandle column);

C_API void DeleteDColumn(const DColumnHandle column);

C_API void DeleteIColumn(const IColumnHandle column);

C_API void DeleteLColumn(const LColumnHandle column);

#ifdef __cplusplus
}
#endif

#endif  // CSVLIBRARY_CINTERFACE_H
