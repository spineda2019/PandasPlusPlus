#ifndef CSVLIBRARY_READCSV_H
#define CSVLIBRARY_READCSV_H

#include <algorithm>
#include <cmath>
#include <complex>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <numeric>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

#include "Exceptions.h"

namespace read_file {

constexpr uint64_t padding = 5;
namespace util {
std::mutex data_mtx;
}

template <class T>
class Dataframe {
 private:
  /**
   * @brief Read the headers from a csv file
   * @param file_has_header: Whether or not a csv file has header row of strings
   * @param file: (non-const) reference to a file stream to read headers
   */
  void ReadHeaders(bool file_has_header, std::ifstream& file) {
    if (file_has_header) {
      std::string header_row;
      std::getline(file, header_row);
      size_t pos{};

      // parse commas in single row
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
    }
  }

  /**
   * @brief Read data from csv and put it into the dataframe
   * @param file: (non-const) reference to the file to read from
   */
  void ReadVals(std::ifstream& file) {
    std::string string_value{};
    std::string sub_string_value{};

    size_t row = 0;
    size_t pos = 0;
    while (std::getline(file, string_value)) {  // Rows
      this->data_.push_back(std::vector<T>{});

      while ((pos = string_value.find(",")) != std::string::npos) {  // Cols
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
   * @brief Fill an empty vector with values (in any order) of a dataframe for
   * the purpose of calculating its Median
   * @param empty_vector: Empty vector to fill with dataframe values
   */
  void Flatten(std::vector<T>& empty_vector) const {
    std::for_each(std::execution::par_unseq, this->data_.begin(),
                  this->data_.end(), [&empty_vector](const std::vector<T>& x) {
                    std::for_each(x.begin(), x.end(), [&x, &empty_vector](T y) {
                      if (!std::isnan(y)) {
                        util::data_mtx.lock();
                        empty_vector.push_back(y);
                        util::data_mtx.unlock();
                      }
                    });
                  });
  }

  /**
   * @brief get median of any vector (Must have NaN pre-handled)
   * @param vec vector with desired median
   * @return median of vector vec
   */
  T MedianHelper(std::vector<T>& vec) const {
    size_t n = vec.size() / 2;
    std::nth_element(vec.begin(), vec.begin() + n, vec.end());

    if (vec.size() % 2 == 0) {
      T left_middle = *std::max_element(vec.begin(), vec.begin() + n);
      return (left_middle + vec[n]) / T(2.0);
    }
    if (vec.size() % 2 != 0) {
      return vec[n];
    }
  }

 public:
  /**
   * @brief Creates a dataframe with a file path and csv headers.
   * @param file_name: filepath to csv file you want to load
   * @param file_has_header: whether or not your file has headers in the first
   * row of the csv
   */
  Dataframe(const std::string& file_name, bool file_has_header)
      : height_{0},
        width_{0},
        max_column_width_{0},
        has_header_row_{file_has_header} {
    std::ifstream file(file_name);
    if (!file) {
      std::cout << "ERROR: INVALID FILE PATH" << std::endl;
      return;
    }

    ReadHeaders(this->has_header_row_, file);

    if (this->max_column_width_ <= 0) {
      this->max_column_width_ = 15;
    }

    ReadVals(file);
  }

  /**
   * @brief Create a basic Dataframe assuming file has headers
   * @param file_name: Name of csv file to read
   */
  Dataframe(const std::string& file_name)
      : height_{0}, width_{0}, max_column_width_{0}, has_header_row_{true} {
    std::ifstream file(file_name);
    if (!file) {
      std::cout << "ERROR: INVALID FILE PATH" << std::endl;
      return;
    }

    ReadHeaders(true, file);

    if (this->max_column_width_ <= 0) {
      this->max_column_width_ = 15;
    }

    ReadVals(file);
  }

  /**
   * @brief Returns the number of rows in a data frame exclusing the header row
   * @return Dataframe's number of rows
   */
  uint64_t Height() const { return this->height_; }

  /**
   * @brief Returns the number of columns in a data frame
   * @return Frame's number of columns
   */
  uint64_t Width() const { return this->width_; }

  /**
   * @brief Returns if the dataframe has a row of column headers or not
   * @return True if there is a row of column headers, False if otherwise
   */
  bool HasHeaderRow() const { return this->has_header_row_; }

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

    // Df Vals
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

    // Df vals
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
  T Mean(const std::string& col_name) const {
    if (!this->has_header_row_) {
      std::cout << "WARNING: This frame has no headers."
                << std::endl;  // throw here
      throw ColumnNotFoundException();
    }

    // Find col index
    auto col_index =
        std::find(std::execution::par_unseq, (this->headers_).begin(),
                  (this->headers_).end(), col_name) -
        (this->headers_).begin();

    if (col_index + (this->headers_).begin() == (this->headers_).end()) {
      std::cout << "ERROR: Column not found" << std::endl;
      throw ColumnNotFoundException();
    }

    // create a basic vector of indices to get mean
    std::vector<T> col(this->height_);

    std::iota(col.begin(), col.end(), T(0.0));

    std::transform(
        std::execution::par_unseq, col.begin(), col.end(), col.begin(),
        [this, &col_index](T& x) {
          T val{};
          // catch NaN
          if (!std::isnan((this->data_)[static_cast<size_t>(x)][col_index])) {
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
  T Mean(const std::string& col_name, bool omit_nan) const {
    if (!omit_nan) {
      return Mean(col_name);
    } else {
      if (!this->has_header_row_) {
        std::cout << "WARNING: This frame has no headers. NaN returned..."
                  << std::endl;  // Throw here
        throw ColumnNotFoundException();
      }

      // Find col index
      auto col_index =
          std::find(std::execution::par_unseq, (this->headers_).begin(),
                    (this->headers_).end(), col_name) -
          (this->headers_).begin();

      if (col_index + (this->headers_).begin() == (this->headers_).end()) {
        std::cout << "ERROR: Column not found" << std::endl;
        throw ColumnNotFoundException();
      }

      // create a basic vector of indices to get mean
      std::vector<T> col(this->height_);

      std::iota(col.begin(), col.end(),
                T(0.0));  // Fix me, complex nums can't ++

      std::transform(
          std::execution::par_unseq, col.begin(), col.end(), col.begin(),
          [this, &col_index](T& x) {
            T val{};
            // catch NaN
            if (!std::isnan((this->data_)[static_cast<size_t>(x)][col_index])) {
              val =
                  (this->data_)[static_cast<size_t>(x)][col_index];  // eval abs
            } else {
              return std::numeric_limits<T>::quiet_NaN();
            }
            return val;
          });

      auto trimmed_col{
          std::views::filter(col, [](T x) { return !std::isnan(x); })};

      uint64_t nans = std::count_if(col.begin(), col.end(),
                                    [](T x) { return std::isnan(x); });

      if (this->height_ - nans == 0) {
        return 0;
      }

      return std::reduce(trimmed_col.begin(), trimmed_col.end()) /
             T(this->height_ - nans);
    }
  }

  /**
   * @brief Get the mean of a specific row of a dataframe
   * @param index: row index to calculate mean
   * @return The row's mean on success, returns NaN on failure
   */
  T Mean(int64_t index) const {
    // Row wise mean
    if (static_cast<uint64_t>(std::abs(index)) >= this->height_) {
      std::cout << "Index out of bounds. NaN returned..."
                << std::endl;  // throw here
      throw DataframeIndexOutOfBoundsException();
    }

    if (index < 0) {
      index = (this->height_) - std::abs(index);
      // std::cout << "Reverse Indexing" << std::endl;
    }

    // extract row to handle NaNs
    // std::vector<T> row = this->data_[index];
    std::vector<T> row(this->width_);
    std::transform(this->data_[index].begin(), this->data_[index].end(),
                   row.begin(), [](const T& x) {
                     if (std::isnan(x)) {
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
  T Mean(int64_t index, bool omit_nan) const {
    if (!omit_nan) {
      return Mean(index);
    } else {
      if (static_cast<uint64_t>(std::abs(index)) >= this->height_) {
        std::cout << "Index out of bounds. NaN returned..."
                  << std::endl;  // throw here
        throw DataframeIndexOutOfBoundsException();
      }

      if (index < 0) {
        index = (this->height_) - std::abs(index);
        // std::cout << "Reverse Indexing" << std::endl;
      }

      auto row{std::ranges::views::filter(
          this->data_[index], [](const T& x) { return !std::isnan(x); })};

      uint64_t nans =
          std::count_if(this->data_[index].begin(), this->data_[index].end(),
                        [](T x) { return std::isnan(x); });

      if (this->width_ - nans == 0) {
        return 0;
      }

      return std::reduce(row.begin(), row.end()) / T(this->width_ - nans);
    }
  }

  /**
   * @brief Get mean of a desired row or column in a dataframe
   * @param index: Index of the vector to get the mean (either column or row)
   * @param omit_nan: Treats NaN as 0 if false, ignores them if true
   * @param col_wise: Whether or not the mean should be calculated column-wise
   * or row-wise
   * @return Mean of desired vector
   */
  T Mean(int64_t index, bool omit_nan, bool col_wise) const {
    if (col_wise && !omit_nan) {
      if (static_cast<uint64_t>(std::abs(index)) >= this->width_) {
        std::cout << "Index out of bounds. NaN returned..."
                  << std::endl;  // throw here
        throw DataframeIndexOutOfBoundsException();
      }

      // negative indexing
      if (index < 0) {
        index = (this->width_) - std::abs(index);
      }

      // create a basic vector of indices to get mean
      std::vector<T> col(this->height_);

      std::iota(col.begin(), col.end(),
                T(0.0));  // Fix me, complex nums can't ++

      std::transform(
          std::execution::par_unseq, col.begin(), col.end(), col.begin(),
          [this, &index](T& x) {
            T val{};
            // catch NaN
            if (!std::isnan((this->data_)[static_cast<size_t>(x)][index])) {
              val = (this->data_)[static_cast<size_t>(x)][index];  // eval abs
            } else {
              val = T(0.0);
            }
            return val;
          });
      return std::reduce(std::execution::par_unseq, col.begin(), col.end()) /
             T(this->height_);
    } else if (col_wise && omit_nan) {
      if (static_cast<uint64_t>(std::abs(index)) >= this->width_) {
        std::cout << "Index out of bounds. NaN returned..."
                  << std::endl;  // throw here
        throw DataframeIndexOutOfBoundsException();
      }

      // negative indexing
      if (index < 0) {
        index = (this->width_) - std::abs(index);
      }

      // create a basic vector of indices to get mean
      std::vector<T> col(this->height_);

      std::iota(col.begin(), col.end(),
                T(0.0));  // Fix me, complex nums can't ++

      std::transform(
          std::execution::par_unseq, col.begin(), col.end(), col.begin(),
          [this, &index](T& x) {
            T val{};
            // catch NaN
            if (!std::isnan((this->data_)[static_cast<size_t>(x)][index])) {
              val = (this->data_)[static_cast<size_t>(x)][index];  // eval abs
            } else {
              return std::numeric_limits<T>::quiet_NaN();
            }
            return val;
          });

      auto trimmed_col{
          std::views::filter(col, [](T x) { return !std::isnan(x); })};

      uint64_t nans = std::count_if(col.begin(), col.end(),
                                    [](T x) { return std::isnan(x); });

      if (this->height_ - nans == 0) {
        return 0;
      }

      return std::reduce(trimmed_col.begin(), trimmed_col.end()) /
             T(this->height_ - nans);
    } else {
      // Row wise mean
      return Mean(index, omit_nan);
    }
  }

  /**
   * @brief return mean of the whole Dataframe assuming NaN omits
   * @return The whole Dataframe's mean
   */
  T Mean(void) const {
    T sum{};
    T divisor{};

    // cannot be threaded, has data race
    std::for_each((this->data_).begin(),  // each row
                  (this->data_).end(),
                  [&sum, &divisor](const std::vector<T>& x) {
                    std::for_each(x.begin(), x.end(), [&sum, &divisor](T y) {
                      if (!std::isnan(y)) {
                        sum += y;
                        divisor++;
                      }
                    });
                  });

    return sum / divisor;
  }

  /**
   * @brief Get mean of a row based on index
   * @param index index of the row to get the median
   * @return median of the desired row
   */
  T Median(int64_t index) const {
    if (static_cast<uint64_t>(std::abs(index)) > this->height_) {
      std::cout << "Index out of bounds. NaN returned..."
                << std::endl;  // throw here
      throw DataframeIndexOutOfBoundsException();
    }

    // negative indexing
    if (index < 0) {
      index = (this->height_) - std::abs(index);
    }

    // Handle NaN
    std::vector<T> row{};
    std::for_each(std::execution::par_unseq, this->data_[index].begin(),
                  this->data_[index].end(), [this, &row](T x) {
                    if (!std::isnan(x)) {
                      util::data_mtx.lock();
                      row.push_back(x);
                      util::data_mtx.unlock();
                    }
                  });

    return MedianHelper(row);
  }

  /**
   * @brief Calculate median value of a column specified by it's header
   * @param col Name of the column with desired median
   * @return Median of named column
   */
  T Median(const std::string& col_name) const {
    // Find col index
    auto col_index =
        std::find(std::execution::par_unseq, (this->headers_).begin(),
                  (this->headers_).end(), col_name) -
        (this->headers_).begin();

    if (col_index + (this->headers_).begin() == (this->headers_).end()) {
      std::cout << "ERROR: Column not found" << std::endl;  // throw here
      throw ColumnNotFoundException();
    }

    std::vector<T> col{};
    std::for_each(this->data_.begin(), this->data_.end(),
                  [&col, col_index](const std::vector<T>& x) {
                    if (!std::isnan(x[col_index])) {
                      col.push_back(x[col_index]);
                    }
                  });

    return MedianHelper(col);
  }

  /**
   * @brief Calculate median value of a column or row specified by it's index
   * @param index Index of column or row
   * @param col_wise Whether or not the index is of a column or row
   * @return Median of selected vector
   */
  T Median(int64_t index, bool col_wise) {
    if (!col_wise) {
      return Median(index);
    } else {
      if (static_cast<uint64_t>(std::abs(index)) > this->width_) {
        std::cout << "Index out of bounds. NaN returned..."
                  << std::endl;  // throw here
        throw DataframeIndexOutOfBoundsException();
      }

      // negative indexing
      if (index < 0) {
        index = (this->width_) - std::abs(index);
      }

      std::vector<T> col{};
      std::for_each(this->data_.begin(), this->data_.end(),
                    [&col, index](const std::vector<T>& x) {
                      if (!std::isnan(x[index])) {
                        col.push_back(x[index]);
                      }
                    });

      return MedianHelper(col);
    }
  }

  /**
   * @brief calculate the composite median value of the whole dataframe
   * @return the median of the whole dataframe
   */
  T Median(void) const {
    std::vector<T> flat{};
    Flatten(flat);
    return MedianHelper(flat);
  }

  /**
   * @brief Insert row from vector at end of dataframe
   * @param row Row to append to end of dataframe
   */
  void InsertRow(const std::vector<T>& row) {
    if (row.size() != this->width_) {
      throw DataframeRowSizeMismatchException();
    }
    this->data_.push_back(row);
    this->height_++;
  }

  template <class V>
  friend inline std::ostream& operator<<(std::ostream& os,
                                         const Dataframe<V>& df);

  std::vector<T> operator[](size_t index) {
    return this->data_[index];  // get
  }

 private:
  size_t height_;
  size_t width_;
  bool has_header_row_;
  std::vector<std::vector<T>> data_;
  std::vector<std::string> headers_;
  size_t max_column_width_;
};  // class Dataframe

template <class V>
inline std::ostream& operator<<(std::ostream& os, const Dataframe<V>& df) {
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