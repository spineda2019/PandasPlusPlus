#include "CCSVInterface.h"

#include "Dataframe.h"

CCSV_API Dataframe create_float_dataframe(const char* file_path,
                                          int file_has_header) {
  return new read_file::Dataframe<float>(file_path, file_has_header);
}