#ifndef CSVLIBRARY_CCSVINTERFACE_H
#define CSVLIBRARY_CCSVINTERFACE_H

#include <stdint.h>

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#ifdef CCSV_EXPORTS
#define CCSV_API __attribute__((dllexport))
#else
#define CCSV_API __attribute__((dllimport))
#endif
#else
#ifdef CCSV_EXPORTS
#define CCSV_API __declspec(dllexport)
#else
#define CCSV_API __declspec(dllimport)
#endif
#endif
#else
#if __GNUC__ >= 4
#define CCSV_API __attribute__((visibility("default")))
#else
#define CCSV_API
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
CCSV_API void read_csv_float(const char* file_name, float* array_to_fill,
                      uint64_t array_to_fill_length);
#ifdef __cplusplus
}
#endif

#endif  // CSVLIBRARY_CCSVINTERFACE_H