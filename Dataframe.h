#ifndef CSVLIBRARY_READCSV_H
#define CSVLIBRARY_READCSV_H

#include <algorithm>
#include <complex>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>
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
  inline uint64_t Height() const { return this->height_; }

  /**
   * @brief Returns the number of columns in a data frame
   * @return Frame's number of columns
   */
  inline uint64_t Width() const { return this->width_; }

  /**
   * @brief Returns if the dataframe has a row of column headers or not
   * @return True if there is a row of column headers, False if otherwise
   */
  inline bool HasHeaderRow() const { return this->has_header_row_; }

  /**
   * @brief Print a desired amount of rows of the data table starting from the
   * top
   * @param n_rows: Number of rows to be printed
   */
  void PrintData(uint64_t n_rows) const {
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
   * at the n to last row
   * @param n_rows: number of rows to print
   */
  void PrintTail(uint64_t n_rows) const {
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
  inline T Mean(const std::string& col_name) const {
    if (!this->has_header_row_) {
      std::cout << "WARNING: This frame has no headers. NaN returned..."
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
   * @brief Get the mean of a specific column of a dataframe
   * @param col_name: String specifying which column to get mean from. Only
   * works if this->has_header_row is true
   * @param omit_nan: Ignores NaNs in all calculations if true, treats them as 0
   * if false
   * @return The column's mean on success, returns NaN on failure
   */
  inline T Mean(const std::string& col_name, bool omit_nan) const {
    if (!omit_nan) {
      return Mean(col_name);
    } else {
      //  if (!this->has_header_row_) {
      //    std::cout << "WARNING: This frame has no headers. NaN returned..."
      //              << std::endl;
      //    return std::numeric_limits<T>::quiet_NaN();
      //  }
      //  // Find col index
      //  auto col_index =
      //      std::find(std::execution::par_unseq, (this->headers_).begin(),
      //                (this->headers_).end(), col_name) -
      //      (this->headers_).begin();
      //
      //  if (col_index + (this->headers_).begin() == (this->headers_).end()) {
      //    std::cout << "ERROR: Column not found" << std::endl;
      //    return std::numeric_limits<T>::quiet_NaN();
      //  }
      //
      //  // create a basic vector of indices to get mean
      //  std::vector<T> mask(this->height_);
      //  std::vector<T> col{};
      //
      //  std::iota(mask.begin(), mask.end(), T(0.0));
      //
      //  std::for_each(
      //      std::execution::par_unseq, mask.begin(), mask.end(),
      //      [this, &col_index](T& x) {
      //        // catch NaN
      //        if ((this->data_)[static_cast<size_t>(x)][col_index] ==
      //            (this->data_)[static_cast<size_t>(x)][col_index]) {
      //          col.push_back((this->data_)[static_cast<size_t>(x)][col_index]);
      //        }
      //      });
      //  return std::reduce(std::execution::par_unseq, col.begin(), col.end())
      //  /
      //         T(col.size());
    }
  }

  /**
   * @brief Get the mean of a specific row of a dataframe
   * @param index: row index to calculate mean
   * @return The row's mean on success, returns NaN on failure
   */
  inline T Mean(size_t index) const {
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

  /**
   * @brief Get the mean of a specific row of a dataframe
   * @param index: row index to calculate mean
   * @param omit_nan: Ignores NaNs on all calculations if true, treats them as 0
   * if false
   * @return The row's mean on success, returns NaN on failure
   */
  inline T Mean(size_t index, bool omit_nan) const {
    if (!omit_nan) {
      return Mean(index);
    } else {
      //  if (index >= this->height_) {
      //    std::cout << "Index out of bounds. NaN returned..." << std::endl;
      //    return std::numeric_limits<T>::quiet_NaN();
      //  }
      //
      //  std::vector<T> row{};
      //  std::for_each((this->data_)[index].begin(),
      //  (this->data_)[index].end(),
      //                [&row](T& x) {
      //                  if (x == x) {
      //                    row.push_back(x);
      //                  }
      //                });
      //
      //  return std::reduce(row.begin(), row.end()) / T(row.size());
    }
  }

  /**
   * @brief Get mean of a desired row or column in a dataframe
   * @param index: Index of the vector to get the mean (either column or row)
   * @param col_wise: Whether or not the mean should be calculated column-wise
   * or row-wise
   * @return Mean of desired vector
   */
  inline T Mean(const size_t index, bool col_wise, bool omit_nan) const {
    if (col_wise && !omit_nan) {
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
    } else if (col_wise && omit_nan) {
      // TODO
    } else {
      // Row wise mean
      return Mean(index, omit_nan);
    }
  }

  template <class T>
  friend inline std::ostream& operator<<(std::ostream& os,
                                         const Dataframe<T>& df);

 private:
  size_t height_;
  size_t width_;
  bool has_header_row_;
  std::vector<std::vector<T>> data_;
  std::vector<std::string> headers_;
  size_t max_column_width_;
};

template <class T>
inline std::ostream& operator<<(std::ostream& os, const Dataframe<T>& df) {
  uint64_t n_rows{};

  if (df.Height() >= 20) {
    n_rows = 10;
  } else {
    n_rows = df.Height() / 2;
  }

  // top
  if (df.HasHeaderRow()) {
    for (size_t i = 0; i < (df.max_column_width_ * df.width_) + (2 * padding);
         i++) {
      os << "_";
    }
    os << std::endl;
    for (const std::string& str : df.headers_) {
      os << std::setw(df.max_column_width_) << str << "|";
    }
    os << std::endl;
  }

  for (size_t i = 0; i < (df.max_column_width_ * df.width_) + (2 * padding);
       i++) {
    os << "_";
  }
  os << std::endl;

  for (size_t row = 0; row < n_rows; row++) {
    for (size_t col = 0; col < df.width_; col++) {
      os << std::setw(df.max_column_width_) << df.data_[row][col] << "|";
    }
    os << std::endl;
  }

  if (n_rows > 1) {
    os << std::endl;
    os << std::setw((df.max_column_width_ * df.Width()) / 2) << "...";
    os << std::endl << std::endl;

    // bottom
    for (size_t row = df.height_ - n_rows; row < df.height_; row++) {
      for (size_t col = 0; col < df.width_; col++) {
        os << std::setw(df.max_column_width_) << df.data_[row][col] << "|";
      }
      os << std::endl;
    }
  }

  return os;
}

}  // namespace read_file

#endif  // CSVLIBRARY_READCSV_H