#include "CCSVInterface.h"

#include "Dataframe.h"

Dataframe create_float_dataframe(const char* file_path,
                                          int file_has_header) {
  return reinterpret_cast<read_file::Dataframe<float>*>(
      new read_file::Dataframe<float>(file_path, file_has_header));
}

void deleteDataframe(Dataframe frame) {
  delete reinterpret_cast<read_file::Dataframe<float>*>(frame);
}