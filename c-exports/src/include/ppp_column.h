#ifndef CSVLIBRARY_CINTERFACE_H
#define CSVLIBRARY_CINTERFACE_H

#include <stddef.h>
#include <stdint.h>

#include "common_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *FColumnHandle;

C_API FColumnHandle NewFColumn(const float *data, size_t length,
                               const char *key);

C_API void PrintFColumn(FColumnHandle column);

C_API FColumnHandle AddFColumns(const FColumnHandle lhs,
                                const FColumnHandle rhs);

C_API void DeleteFColumn(const FColumnHandle column);

#ifdef __cplusplus
}
#endif

#endif  // CSVLIBRARY_CINTERFACE_H
