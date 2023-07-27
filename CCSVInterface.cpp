#include "CCSVInterface.h"

#include <stdint.h>

#include "Dataframe.h"

Dataframe create_dataframe_float(const char* file_path, int file_has_header) {
  return reinterpret_cast<read_file::Dataframe<float>*>(
      new read_file::Dataframe<float>(file_path, file_has_header));
}

void delete_Dataframe_float(Dataframe frame) {
  delete reinterpret_cast<read_file::Dataframe<float>*>(frame);
}

void print_Dataframe_tail_float(Dataframe frame, uint64_t n_rows) {
  reinterpret_cast<read_file::Dataframe<float>*>(frame)->PrintTail(n_rows);
}

void print_Dataframe_head_float(Dataframe frame, uint64_t n_rows) {
  reinterpret_cast<read_file::Dataframe<float>*>(frame)->PrintData(n_rows);
}