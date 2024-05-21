#ifndef CSVLIBRARY_CINTERFACE_H
#define CSVLIBRARY_CINTERFACE_H

#include <stddef.h>
#include <stdint.h>

#if defined _WIN32 || defined __CYGWIN__              // *
#ifdef __GNUC__                                       // **
#ifdef C_EXPORTS                                      // ***
#define C_API __attribute__((dllexport))              // ***
#else                                                 // ***
#define C_API __attribute__((dllimport))              // ***
#endif                                                // ***
#else                                                 // **
#ifdef C_EXPORTS                                      // ***
#define C_API __declspec(dllexport)                   // ***
#else                                                 // ***
#define C_API __declspec(dllimport)                   // ***
#endif                                                // ***
#endif                                                // **
#else                                                 // *
#if __GNUC__ >= 4                                     // **
#define C_API __attribute__((visibility("default")))  // **
#else                                                 // **
#define C_API                                         // **
#endif                                                // **
#endif                                                // *

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
