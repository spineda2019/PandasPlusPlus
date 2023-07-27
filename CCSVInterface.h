#ifndef CSVLIBRARY_CCSVINTERFACE_H
#define CSVLIBRARY_CCSVINTERFACE_H

#include <stdint.h>

#include "Dataframe.h"

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
typedef void* Dataframe;

CCSV_API Dataframe create_float_dataframe(const char* file_path,
                                     int file_has_header);

#ifdef __cplusplus
}
#endif

#endif  // CSVLIBRARY_CCSVINTERFACE_H