#ifndef CSVLIBRARY_READCSV_H
#define CSVLIBRARY_READCSV_H

#include <algorithm>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace read_file {

constexpr uint64_t padding = 5;

template <class T>
class Dataframe {
 public:
  /**
   * @brief Creates a dataframe with a file path and csv headers.
   * @param file_name: filepath to csv file you want to load
   * @param file_has_header: whether or not your file has headers in the first
   * row of the csv
   */
  [[nodiscard]] explicit Dataframe(const std::string& file_name,
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
        this->headers_.push_back(header_row.substr(0, header_row.find(",")));

        if (header_row.substr(0, header_row.find(",")).length() >
            this->max_column_width_) {
          this->max_column_width_ =
              header_row.substr(0, header_row.find(",")).length() + padding;
        }

        header_row.erase(0, pos + 1);
      }
      this->headers_.push_back(
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
    size_t pos = 0;
    while (std::getline(file, string_value)) {
      this->data_.push_back(std::vector<T>{});

      while ((pos = string_value.find(",")) != std::string::npos) {
        this->width_++;

        try {
          this->data_[row].push_back(
              T(std::stof(string_value.substr(0, string_value.find(",")))));
        } catch (...) {
          this->data_[row].push_back(std::numeric_limits<T>::quiet_NaN());
        }
        string_value.erase(0, pos + 1);
      }

      // store last col val where no more comma is found
      try {
        this->data_[row].push_back(T(std::stof(string_value)));
      } catch (...) {
        this->data_[row].push_back(std::numeric_limits<T>::quiet_NaN());
      }
      this->width_++;

      row++;
      this->height_++;
    }
    this->width_ = (this->width_ / this->height_);
  }

  /**
   * @brief Returns the number of rows in a data frame exclusing the header row
   * @return Dataframe's number of rows
   */
  inline uint64_t Height() { return this->height_; }

  /**
   * @brief Returns the number of columns in a data frame
   * @return Frame's number of columns
   */
  inline uint64_t Width() { return this->width_; }

  /**
   * @brief Returns if the dataframe has a row of column headers or not
   * @return True if there is a row of column headers, False if otherwise
   */
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
      for (size_t i = 0;
           i < (this->max_column_width_ * this->width_) + (2 * padding); i++) {
        std::cout << "_";
      }
      std::cout << std::endl;
      for (const std::string& str : this->headers_) {
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
        std::cout << std::setw(this->max_column_width_) << this->data_[row][col]
                  << "|";
      }
      std::cout << std::endl;
    }
  }

  /**
   * @brief Print the last n rows of the dataframe to standard output, starting
   * at the last row
   * @param n_rows: number of rows to print
   */
  void PrintTail(uint64_t n_rows) {
    if (n_rows > this->height_) {
      std::cout << "More rows requested than exist" << std::endl;
      return;
    }

    if (n_rows < 0) {
      std::cout << "Can not print negative rows" << std::endl;
      return;
    }

    if (this->HasHeaderRow()) {
      for (size_t i = 0;
           i < (this->max_column_width_ * this->width_) + (2 * padding); i++) {
        std::cout << "_";
      }
      std::cout << std::endl;
      for (const std::string& str : this->headers_) {
        std::cout << std::setw(this->max_column_width_) << str << "|";
      }
      std::cout << std::endl;
    }

    for (size_t i = 0;
         i < (this->max_column_width_ * this->width_) + (2 * padding); i++) {
      std::cout << "_";
    }
    std::cout << std::endl;

    for (size_t row = this->height_ - n_rows; row < this->height_; row++) {
      for (size_t col = 0; col < this->width_; col++) {
        std::cout << std::setw(this->max_column_width_) << this->data_[row][col]
                  << "|";
      }
      std::cout << std::endl;
    }
  }

  /**
   * @brief Get the mean of a specific column of that dataframe
   * @param col: String specifying which column to get mean from. Only works if
   * this->has_header_row is true
   * @return The column's mean on success, returns NaN on failure
   */
  T Mean(const std::string& col_name) {
    if (!this->has_header_row_) {
      std::cout << "This frame has no headers. Be aware, NaN returned..."
                << std::endl;
      return std::numeric_limits<T>::quiet_NaN();
    }
    // Find col index
    auto col_index =
        std::find(std::execution::par_unseq, (this->headers_).begin(),
                  (this->headers_).end(), col_name) -
        (this->headers_).begin();

    if (col_index + (this->headers_).begin() == (this->headers_).end()) {
      std::cout << "ERROR: Column not found" << std::endl;
      return std::numeric_limits<T>::quiet_NaN();
    }

    // create a basic vector of indices to get mean
    std::vector<T> col(this->height_);

    std::iota(col.begin(), col.end(), T(0.0));

    std::transform(std::execution::par_unseq, col.begin(), col.end(),
                   col.begin(), [this, &col_index](T& x) {
                     T val{};
                     // catch NaN
                     if ((this->data_)[static_cast<size_t>(x)][col_index] ==
                         (this->data_)[static_cast<size_t>(x)][col_index]) {
                       val = (this->data_)[static_cast<size_t>(x)][col_index];
                     } else {
                       val = T(0.0);
                     }
                     return val;
                   });
    return std::reduce(std::execution::par_unseq, col.begin(), col.end()) /
           T(this->height_);
  }

  /**
   * @brief
   * @param index
   * @param vertical
   * @return
   */
  T Mean(const size_t index, bool col_wise) {
    if (col_wise) {
      if (index >= this->width_) {
        std::cout << "Index out of bounds. NaN returned..." << std::endl;
        return std::numeric_limits<T>::quiet_NaN();
      }

      // create a basic vector of indices to get mean
      std::vector<T> col(this->height_);

      std::iota(col.begin(), col.end(), T(0.0));

      std::transform(std::execution::par_unseq, col.begin(), col.end(),
                     col.begin(), [this, &index](T& x) {
                       T val{};
                       // catch NaN
                       if ((this->data_)[static_cast<size_t>(x)][index] ==
                           (this->data_)[static_cast<size_t>(x)][index]) {
                         val = (this->data_)[static_cast<size_t>(x)][index];
                       } else {
                         val = T(0.0);
                       }
                       return val;
                     });
      return std::reduce(std::execution::par_unseq, col.begin(), col.end()) /
             T(this->height_);
    } else {
      // Row wise mean
      if (index >= this->height_) {
        std::cout << "Index out of bounds. NaN returned..." << std::endl;
        return std::numeric_limits<T>::quiet_NaN();
      }

      // extract row to handle NaNs
      std::vector<T> row = this->data_[index];
      std::transform(row.begin(), row.end(), row.begin(), [](T& x) {
        if (x != x) {
          return T(0.0);
        } else {
          return x;
        }
      });

      return std::reduce(row.begin(), row.end()) / T(this->width_);
    }
  }

  T Mean() {}

 private:
  uint64_t height_;
  uint64_t width_;
  bool has_header_row_;
  std::vector<std::vector<T>> data_;
  std::vector<std::string> headers_;
  uint64_t max_column_width_;
};

}  // namespace read_file

#endif  // CSVLIBRARY_READCSV_H