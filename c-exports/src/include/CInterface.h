#ifndef CSVLIBRARY_CINTERFACE_H
#define CSVLIBRARY_CINTERFACE_H

#include <stdint.h>

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#ifdef C_EXPORTS
#define C_API __attribute__((dllexport))
#else
#define C_API __attribute__((dllimport))
#endif
#else
#ifdef C_EXPORTS
#define C_API __declspec(dllexport)
#else
#define C_API __declspec(dllimport)
#endif
#endif
#else
#if __GNUC__ >= 4
#define C_API __attribute__((visibility("default")))
#else
#define C_API
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
typedef struct Dataframe_t {
    unsigned long long height_;
    unsigned long long width_;
    unsigned char has_header_row_;  // not bool to keep C/C++ compatibility
    float **data_;                  // 2d array of Floats
    char **headers_;                // array of strings
    unsigned long long max_column_width_;
} Dataframe;
// Float methods

C_API Dataframe *create_dataframe_float(const char *file_path,
                                        unsigned char file_has_header);

inline C_API void delete_Dataframe_float(Dataframe *frame);

inline C_API void print_Dataframe_tail_float(Dataframe *frame, uint64_t n_rows);

inline C_API void print_Dataframe_head_float(Dataframe *frame, uint64_t n_rows);

inline C_API float col_name_mean_float(Dataframe *frame, const char *col_name);

#ifdef __cplusplus
}
#endif

#endif  // CSVLIBRARY_CINTERFACE_H
