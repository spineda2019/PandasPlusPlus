#ifndef CSVLIBRARY_READCSV_H
#define CSVLIBRARY_READCSV_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace read_file {

constexpr uint64_t padding = 5;

template <class T>
class CSV {
 public:
  /**
   * @brief Creates a dataframe with a file path and csv headers.
   * @param file_name: filepath to csv file you want to load
   * @param file_has_header: whether or not your file has headers in the first
   * row of the csv
   */
  [[nodiscard]] explicit CSV(const std::string& file_name,
                             bool file_has_header) {
    this->height_ = 0;
    this->width_ = 0;
    this->max_column_width_ = 0;

    std::ifstream file(file_name);

    if (!file) {
      std::cout << "INVALID FILE PATH" << std::endl;
      return;
    }

    if (file_has_header) {
      this->has_header_row_ = true;
      std::string header_row;
      std::getline(file, header_row);
      size_t pos{};

      while ((pos = header_row.find(",")) != std::string::npos) {
        this->headers.push_back(header_row.substr(0, header_row.find(",")));
        if (header_row.substr(0, header_row.find(",")).length() >
            this->max_column_width_) {
          this->max_column_width_ =
              header_row.substr(0, header_row.find(",")).length() + padding;
        }
        header_row.erase(0, pos + 1);
      }
      this->headers.push_back(
          header_row);  // push last val since it will no longer have a comma
    } else {
      this->has_header_row_ = false;
    }

    if (this->max_column_width_ <= 0) {
      max_column_width_ = 15;
    }

    std::string string_value{};
    std::string sub_string_value{};

    size_t row = 0;
    size_t pos{};
    while (std::getline(file, string_value)) {
      this->data.push_back(std::vector<T>{});

      while ((pos = string_value.find(",")) != std::string::npos) {
        this->width_++;
        try {
          this->data[row].push_back(
              T(std::stof(string_value.substr(0, string_value.find(",")))));
        } catch (...) {
          this->data[row].push_back(T(0));
        }
        string_value.erase(0, pos + 1);
      }
      row++;
      this->height_++;
    }
    this->width_ = (this->width_ / this->height_) + 1;
  }

  inline uint64_t Height() { return this->height_; }
  inline uint64_t Width() { return this->width_; }
  inline bool HasHeaderRow() { return this->has_header_row_; }

  /**
   * @brief Print a desired amount of rows of the data table starting from the
   * top
   * @param n_rows: Number of rows to be printed
   */
  void PrintData(uint64_t n_rows) {
    if (n_rows > this->height_) {
      std::cout << "More rows requested than exist" << std::endl;
      return;
    }

    if (n_rows < 0) {
      std::cout << "Can not print negative rows" << std::endl;
      return;
    }

    if (this->HasHeaderRow()) {
      for (const std::string& str : this->headers) {
        std::cout << std::setw(this->max_column_width_) << str << "|";
      }
      std::cout << std::endl;
    }

    for (size_t i = 0;
         i < (this->max_column_width_ * this->width_) + (2 * padding); i++) {
      std::cout << "_";
    }
    std::cout << std::endl;

    for (size_t row = 0; row < n_rows; row++) {
      for (size_t col = 0; col < this->width_; col++) {
        std::cout << std::setw(this->max_column_width_) << this->data[row][col]
                  << "|";
      }
      std::cout << std::endl;
    }
  }

  void PrintTail(uint64_t n_rows) {
    if (n_rows > this->height_) {
      std::cout << "More rows requested than exist" << std::endl;
      return;
    }

    if (n_rows < 0) {
      std::cout << "Can not print negative rows" << std::endl;
      return;
    }

    for (size_t row = this->height_ - 1; row > this->height_ - n_rows; row--) {
      for (size_t col = 0; col < this->width_; col++) {
        std::cout << std::setw(this->max_column_width_) << this->data[row][col]
                  << "|";
      }
      std::cout << std::endl;
    }
  }

 private:
  uint64_t height_;
  uint64_t width_;
  bool has_header_row_;
  std::vector<std::vector<T>> data;
  std::vector<std::string> headers;
  uint64_t max_column_width_;
};

}  // namespace read_file

#endif  // CSVLIBRARY_READCSV_H