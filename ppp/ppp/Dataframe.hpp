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
#include <vector>

#include "Exceptions.hpp"

namespace read_file {

constexpr uint64_t padding = 5;
namespace detail {
static std::mutex data_mtx{}; // needs to be static for one definition rule when
                              // used in other projects

template <class T>
inline void CalculateFrequencyDomain(const std::vector<T> &signal,
                                     std::vector<T> &array_to_fill,
                                     size_t sample_rate) {
  std::iota(array_to_fill.begin(), array_to_fill.end(), T(0));

  T scaler = static_cast<T>(sample_rate) / signal.size();
  std::transform(std::execution::par, array_to_fill.begin(),
                 array_to_fill.end(), array_to_fill.begin(),
                 [&scaler](T &x) { return x * scaler; });
}

/**
 * @brief Calculate Discrete Fourier Transform of a time domain waveform
 *
 * @param[in] signal: waveform to transform
 * @param[out] domain_to_fill: non-empty pre-filled destination vector
 *
 */
template <class T>
inline void CalculateDiscreteFT(const std::vector<T> &signal,
                                std::vector<std::complex<T>> &domain_to_fill) {
  std::vector<std::vector<std::complex<T>>> k(signal.size()); // Column vec

  // init k w/ 0,0
  constexpr std::complex<T> zero_z(T(0.0), T(0.0));
  std::fill(std::execution::par_unseq, k.begin(), k.end(),
            std::vector<std::complex<T>>{zero_z});

  // multiply k.T * k to create a square matrix
  for (size_t row = 0; row < k.size(); row++) {
    for (size_t col = 1; col < k.size(); col++) {
      k[row].push_back(std::complex<T>(T(col) * T(row), T(0)));
    }
  }

  // e = exp(-2j * pi * k.T * k / N)
  constexpr std::complex<T> comp(T(0.0), T(-2.0));
  const T N = T(k.size());
  std::for_each(
      std::execution::par_unseq, k.begin(), k.end(), [&comp, &N](auto &x) {
        std::transform(std::execution::par, x.begin(), x.end(), x.begin(),
                       [&comp, &N](std::complex<T> &y) {
                         return std::pow(T(2.71828),
                                         comp * T(3.141592654) * y / N);
                       });
      });

  // e dot signal
  std::transform(std::execution::par_unseq, k.begin(), k.end(),
                 domain_to_fill.begin(), [&signal, &N](auto &x) {
                   const std::complex<T> dot =
                       std::inner_product(x.begin(), x.end(), signal.begin(),
                                          std::complex<T>(T(0.0), T(0.0)));
                   return dot / (std::complex<T>(N) /
                                 std::complex<T>(T(2.0), T(0.0)));
                 });
}

/**
 * @brief Extract N frequencies from a signal from highest amplitude to lowest
 *
 * @param[in] signal: waveform to extract frequencies
 * @param[out] dest: empty destination vector for extracted frequencies
 * @param[in] sample_rate: sample rate of signal
 * @param[in] n: desired number of frequencies to get (optional, defaults to
 * 3)
 *
 */
template <class T>
inline void ExtractNFrequencies(const std::vector<T> &signal,
                                std::vector<T> &dest, const size_t sample_rate,
                                const size_t n) {
  if (dest.size()) {
    std::cout << "Error: Non-empty destination vector received" << std::endl;
    return;
  }

  std::vector<T> frequency_domain(signal.size());
  CalculateFrequencyDomain(signal, frequency_domain, sample_rate);

  std::vector<std::complex<T>> dft_domain(signal.size());
  CalculateDiscreteFT(signal, dft_domain);

  std::vector<T> real(signal.size() / 2);
  std::transform(std::execution::par_unseq, dft_domain.begin(),
                 dft_domain.begin() + real.size(), real.begin(),
                 [](std::complex<T> &x) { return std::abs(x); });

  auto it = std::max_element(real.begin(), real.end());
  for (size_t i = 0; i < n; i++) {
    // value on freq domain at index of highest val in dft domain
    dest.push_back(frequency_domain[std::distance(real.begin(), it)]);

    real[std::distance(real.begin(), it)] = 0;

    it = std::max_element(real.begin(), real.end());
  }
}

/**
 * @brief Extract N amplitudes from a signal from highest amplitude to lowest
 *
 * @param[in] signal: waveform to extract amplitudes
 * @param[out] dest: empty destination vector for extracted amplitudes
 * @param[in] n: desired number of amplitudes to get (optional, defaults to 3)
 *
 */
template <class T>
inline void ExtractNAmplitudes(const std::vector<T> &signal,
                               std::vector<T> &dest, const size_t n) {
  if (dest.size()) {
    std::cout << "Error: Non-empty destination vector received" << std::endl;
    return;
  }

  std::vector<std::complex<T>> dft_domain(signal.size());
  CalculateDiscreteFT(signal, dft_domain);

  std::vector<T> real(signal.size() / 2);
  std::transform(std::execution::par_unseq, dft_domain.begin(),
                 dft_domain.begin() + real.size(), real.begin(),
                 [](std::complex<T> &x) { return std::abs(x); });

  auto it = std::max_element(real.begin(), real.end());
  for (size_t i = 0; i < n; i++) {
    dest.push_back(*it);

    real[std::distance(real.begin(), it)] = 0;

    it = std::max_element(real.begin(), real.end());
  }
}

/**
 * @brief Extract N phases from a signal from highest amplitude to lowest
 *
 * @param[in] signal: waveform to extract phases
 * @param[out] dest: empty destination vector for extracted phases
 * @param[in] n: desired number phases to get (optional, defaults to 3)
 *
 */
template <class T>
inline void ExtractNPhases(const std::vector<T> &signal, std::vector<T> &dest,
                           const size_t n) {
  if (dest.size()) {
    std::cout << "Error: Non-empty destination vector received" << std::endl;
    return;
  }

  std::vector<std::complex<T>> dft_domain(signal.size());
  CalculateDiscreteFT(signal, dft_domain);

  std::vector<T> real(signal.size() / 2);
  std::transform(std::execution::par_unseq, dft_domain.begin(),
                 dft_domain.begin() + real.size(), real.begin(),
                 [](std::complex<T> &x) { return std::abs(x); });

  std::complex<T> max_el{};
  auto it = std::max_element(real.begin(), real.end());
  for (size_t i = 0; i < n; i++) {
    max_el = dft_domain[std::distance(real.begin(), it)];

    dest.push_back(std::atan2(max_el.imag(), max_el.real()));

    real[std::distance(real.begin(), it)] = 0;

    it = std::max_element(real.begin(), real.end());
  }
}
} // namespace detail

template <class T>
concept AlgebraicTerm = requires(T value) {
  value == value;
  value = -value;
  value = value + 1;
  value = value - 1;
};

template <AlgebraicTerm T> class Dataframe {
private:
  /**
   * @brief Read the headers from a csv file
   * @param file_has_header: Whether or not a csv file has header row of strings
   * @param file: (non-const) reference to a file stream to read headers
   */
  void ReadHeaders(bool file_has_header, std::ifstream &file) {
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
          header_row); // push last val since it will no longer have a comma
    }
  }

  /**
   * @brief Read data from csv and put it into the dataframe
   * @param file: (non-const) reference to the file to read from
   */
  void ReadVals(std::ifstream &file) {
    std::string string_value{};
    std::string sub_string_value{};

    size_t row = 0;
    size_t pos = 0;
    while (std::getline(file, string_value)) { // Rows
      this->data_.push_back(std::vector<T>{});

      while ((pos = string_value.find(",")) != std::string::npos) { // Cols
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
  void Flatten(std::vector<T> &empty_vector) const {
    std::for_each(std::execution::par_unseq, this->data_.begin(),
                  this->data_.end(), [&empty_vector](const std::vector<T> &x) {
                    std::for_each(x.begin(), x.end(), [&x, &empty_vector](T y) {
                      if (!std::isnan(y)) {
                        detail::data_mtx.lock();
                        empty_vector.push_back(y);
                        detail::data_mtx.unlock();
                      }
                    });
                  });
  }

  /**
   * @brief get median of any vector (Must have NaN pre-handled)
   * @param vec vector with desired median
   * @return median of vector vec
   */
  T MedianHelper(std::vector<T> &vec) const {
    size_t n = vec.size() / 2;
    std::nth_element(vec.begin(), vec.begin() + n, vec.end());

    if (vec.size() % 2 == 0) {
      T left_middle = *std::max_element(vec.begin(), vec.begin() + n);
      return (left_middle + vec[n]) / T(2.0);
    }
    if (vec.size() % 2 != 0) {
      return vec[n];
    } else {
    }
  }

public:
  /**
   * @brief Creates a dataframe with a file path and csv headers.
   * @param file_name: filepath to csv file you want to load
   * @param file_has_header: whether or not your file has headers in the first
   * row of the csv
   */
  Dataframe(const std::string &file_name, bool file_has_header)
      : height_{0}, width_{0}, max_column_width_{0},
        has_header_row_{file_has_header} {
    std::ifstream file(file_name);
    if (!file) {
      std::cerr << "ERROR: INVALID FILE PATH" << std::endl;
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
  Dataframe(const std::string &file_name)
      : height_{0}, width_{0}, max_column_width_{0}, has_header_row_{true} {
    std::ifstream file(file_name);
    if (!file) {
      std::cerr << "ERROR: INVALID FILE PATH" << std::endl;
      return;
    }

    ReadHeaders(true, file);

    if (this->max_column_width_ <= 0) {
      this->max_column_width_ = 15;
    }

    ReadVals(file);
  }

  /**
   * @brief Create a dataframe from local vectors without headers
   * @param local_data Array to create Dataframe from
   */
  Dataframe(const std::vector<std::vector<T>> &local_data)
      : height_{local_data.size()}, width_{local_data[0].size()},
        data_{local_data}, headers_{std::vector<std::string>{}},
        max_column_width_{15}, has_header_row_{false} {
    for (const std::vector<T> &row : local_data) {
      if (row.size() != this->width_) {
        throw BadDataframeShapeException();
      }
    }
  }

  /**
   * @brief Create a dataframe from local vectors with headers
   * @param local_data Array to create Dataframe from
   */
  Dataframe(const std::vector<std::vector<T>> &local_data,
            const std::vector<std::string> &headers)
      : height_{local_data.size()}, width_{local_data[0].size()},
        data_{local_data}, headers_{headers}, max_column_width_{15},
        has_header_row_{true} {
    for (const std::vector<T> &row : local_data) {
      if (row.size() != this->width_) {
        throw BadDataframeShapeException();
      }
    }

    if (headers.size() != this->width_) {
      throw HeaderDataSizeMismatchException();
    }

    for (const std::string &label : headers) {
      if (label.length() > 15) {
        this->max_column_width_ = label.length();
      }
    }
  }

  /**
   * @brief CTOR to make an empty dataframe
   */
  Dataframe()
      : height_{0}, width_{0}, data_{std::vector<std::vector<T>>{}},
        headers_{std::vector<std::string>{}}, max_column_width_{15},
        has_header_row_{false} {}

  /**
   * @brief Create an empty dataframe with a pre-chosen size and no headers
   * @param rows Number of desired Rows in Dataframe
   * @param columns Number of desired Columns in Dataframe
   */
  Dataframe(size_t rows, size_t columns)
      : height_{rows}, width_{columns},
        data_{
            std::vector<std::vector<T>>(rows, std::vector<T>(columns, T(0.0)))},
        headers_{std::vector<std::string>{}}, max_column_width_{15},
        has_header_row_{false} {}

  /**
   * @brief Create an empty dataframe with a pre-chosen size with headers
   * @param rows Number of desired Rows in Dataframe
   * @param columns Number of desired Columns in Dataframe
   * @param headers Vector of headers of the Dataframe
   */
  Dataframe(size_t rows, size_t columns,
            const std::vector<std::string> &headers)
      : height_{rows}, width_{columns},
        data_{
            std::vector<std::vector<T>>(rows, std::vector<T>(columns, T(0.0)))},
        headers_{headers}, max_column_width_{15}, has_header_row_{true} {
    if (headers.size() != this->width_) {
      throw HeaderDataSizeMismatchException();
    }

    for (const std::string &label : headers) {
      if (label.length() > this->max_column_width_) {
        this->max_column_width_ = label.length();
      }
    }
  }

  /**
   * @brief
   * @param new_header_row
   */
  void RefactorHeaders(const std::vector<std::string> &new_header_row) {
    if (this->has_header_row_) {
      // check size mismatch
      if (new_header_row.size() != this->headers_.size()) {
        throw HeaderDataSizeMismatchException();
      }

      this->headers_ = new_header_row;
    } else {
      if (new_header_row.size() != this->width_) {
        throw HeaderDataSizeMismatchException();
      }

      this->has_header_row_ = true;
      this->headers_ = new_header_row;
    }

    // update column width for printing
    for (const std::string &label : new_header_row) {
      if (label.length() > this->max_column_width_) {
        this->max_column_width_ = label.length();
      }
    }
  }

  /**
   * @brief Change a Dataframe's column given a vector and a numeric index
   * @param index Numeric index of the column to refactor
   * @param new_column The new column replacing the old column
   */
  void RefactorColumn(size_t index, const std::vector<T> &new_column) {
    if (new_column.size() != this->height_) {
      throw DataframeVectorSizeMismatchException();
    }

    if (index >= this->width_) {
      throw DataframeIndexOutOfBoundsException();
    }

    for (size_t row = 0; row < this->height_; row++) {
      this->data_[row][index] = new_column[row];
    }

    // std::for_each(std::execution::par_unseq, this->data_.begin(),
    //               this->data_.end(),
    //               [&new_column, this, index](std::vector<T>& x) {
    //                 x[index] = new_column[x - this->data_.begin()];
    //               });
  }

  /**
   * @brief Change a Dataframe's column given a vector and a numeric index
   * @param index String index of the column to refactor
   * @param new_column The new column replacing the old column
   */
  void RefactorColumn(const std::string &index,
                      const std::vector<T> &new_column) {
    // TODO: String indexed refactor
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

      for (const std::string &str : this->headers_) {
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

      for (const std::string &str : this->headers_) {
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
  T Mean(const std::string &col_name) const {
    if (!this->has_header_row_) {
      std::cout << "WARNING: This frame has no headers."
                << std::endl; // throw here
      throw ColumnNotFoundException();
    }

    // Find col index
    auto col_index =
        std::find(std::execution::par_unseq, (this->headers_).begin(),
                  (this->headers_).end(), col_name) -
        (this->headers_).begin();

    if (col_index + (this->headers_).begin() == (this->headers_).end()) {
      std::cerr << "ERROR: Column not found" << std::endl;
      throw ColumnNotFoundException();
    }

    // create a basic vector of indices to get mean
    std::vector<T> col(this->height_);

    std::iota(col.begin(), col.end(), T(0.0));

    std::transform(
        std::execution::par_unseq, col.begin(), col.end(), col.begin(),
        [this, &col_index](T &x) {
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
  T Mean(const std::string &col_name, bool omit_nan) const {
    if (!omit_nan) {
      return Mean(col_name);
    } else {
      if (!this->has_header_row_) {
        std::cout << "WARNING: This frame has no headers. NaN returned..."
                  << std::endl; // Throw here
        throw ColumnNotFoundException();
      }

      // Find col index
      auto col_index =
          std::find(std::execution::par_unseq, (this->headers_).begin(),
                    (this->headers_).end(), col_name) -
          (this->headers_).begin();

      if (col_index + (this->headers_).begin() == (this->headers_).end()) {
        std::cerr << "ERROR: Column not found" << std::endl;
        throw ColumnNotFoundException();
      }

      // create a basic vector of indices to get mean
      std::vector<T> col(this->height_);

      std::iota(col.begin(), col.end(),
                T(0.0)); // Fix me, complex nums can't ++

      std::transform(
          std::execution::par_unseq, col.begin(), col.end(), col.begin(),
          [this, &col_index](T &x) {
            T val{};
            // catch NaN
            if (!std::isnan((this->data_)[static_cast<size_t>(x)][col_index])) {
              val =
                  (this->data_)[static_cast<size_t>(x)][col_index]; // eval abs
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
                << std::endl; // throw here
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
                   row.begin(), [](const T &x) {
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
                  << std::endl; // throw here
        throw DataframeIndexOutOfBoundsException();
      }

      if (index < 0) {
        index = (this->height_) - std::abs(index);
        // std::cout << "Reverse Indexing" << std::endl;
      }

      auto row{std::ranges::views::filter(
          this->data_[index], [](const T &x) { return !std::isnan(x); })};

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
                  << std::endl; // throw here
        throw DataframeIndexOutOfBoundsException();
      }

      // negative indexing
      if (index < 0) {
        index = (this->width_) - std::abs(index);
      }

      // create a basic vector of indices to get mean
      std::vector<T> col(this->height_);

      std::iota(col.begin(), col.end(),
                T(0.0)); // Fix me, complex nums can't ++

      std::transform(
          std::execution::par_unseq, col.begin(), col.end(), col.begin(),
          [this, &index](T &x) {
            T val{};
            // catch NaN
            if (!std::isnan((this->data_)[static_cast<size_t>(x)][index])) {
              val = (this->data_)[static_cast<size_t>(x)][index]; // eval abs
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
                  << std::endl; // throw here
        throw DataframeIndexOutOfBoundsException();
      }

      // negative indexing
      if (index < 0) {
        index = (this->width_) - std::abs(index);
      }

      // create a basic vector of indices to get mean
      std::vector<T> col(this->height_);

      std::iota(col.begin(), col.end(),
                T(0.0)); // Fix me, complex nums can't ++

      std::transform(
          std::execution::par_unseq, col.begin(), col.end(), col.begin(),
          [this, &index](T &x) {
            T val{};
            // catch NaN
            if (!std::isnan((this->data_)[static_cast<size_t>(x)][index])) {
              val = (this->data_)[static_cast<size_t>(x)][index]; // eval abs
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
    std::for_each((this->data_).begin(), // each row
                  (this->data_).end(),
                  [&sum, &divisor](const std::vector<T> &x) {
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
                << std::endl; // throw here
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
                      detail::data_mtx.lock();
                      row.push_back(x);
                      detail::data_mtx.unlock();
                    }
                  });

    return MedianHelper(row);
  }

  /**
   * @brief Calculate median value of a column specified by it's header
   * @param col Name of the column with desired median
   * @return Median of named column
   */
  T Median(const std::string &col_name) const {
    // Find col index
    auto col_index =
        std::find(std::execution::par_unseq, (this->headers_).begin(),
                  (this->headers_).end(), col_name) -
        (this->headers_).begin();

    if (col_index + (this->headers_).begin() == (this->headers_).end()) {
      std::cerr << "ERROR: Column not found" << std::endl; // throw here
      throw ColumnNotFoundException();
    }

    std::vector<T> col{};
    std::for_each(this->data_.begin(), this->data_.end(),
                  [&col, col_index](const std::vector<T> &x) {
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
                  << std::endl; // throw here
        throw DataframeIndexOutOfBoundsException();
      }

      // negative indexing
      if (index < 0) {
        index = (this->width_) - std::abs(index);
      }

      std::vector<T> col{};
      std::for_each(this->data_.begin(), this->data_.end(),
                    [&col, index](const std::vector<T> &x) {
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
  void InsertRow(const std::vector<T> &row) {
    if (row.size() != this->width_) {
      throw DataframeVectorSizeMismatchException();
    }
    this->data_.push_back(row);
    this->height_++;
  }

  /**
   * @brief Insert an empty row to the end of a Dataframe
   */
  void InsertRow() {
    std::vector<T> empty(this->width_);
    std::fill(empty.begin(), empty.end(), T(0.0));

    this->data_.push_back(empty);
    this->height_++;
  }

  /**
   * @brief Insert a new named column into the data frame
   * @param header String for the header of this new column
   * @param col the vector to be inserted into the datafarme
   */
  void InsertColumn(const std::string &header, const std::vector<T> &col) {
    if (col.size() != this->height_) {
      throw DataframeVectorSizeMismatchException();
    }

    if (!this->has_header_row_) {
      throw HeaderStateMismatchException();
    }

    this->headers_.push_back(header);

    for (size_t row = 0; row < this->height_; row++) {
      this->data_[row].push_back(col[row]);
    }

    this->width_++;
  }

  /**
   * @brief Insert a new unnamed column into the dataframe
   * @param col The vector to insert into the dataframe
   */
  void InsertColumn(const std::vector<T> &col) {
    if (col.size() != this->height_) {
      throw DataframeVectorSizeMismatchException();
    }

    if (this->has_header_row_) {
      throw HeaderStateMismatchException();
    }

    for (size_t row = 0; row < this->height_; row++) {
      this->data_[row].push_back(col[row]);
    }

    this->width_++;
  }

  /**
   * @brief Insert new empty named column into dataframe
   */
  void InsertColumn(const std::string &header) {
    if (!this->has_header_row_) {
      throw HeaderStateMismatchException();
    }

    this->headers_.push_back(header);

    std::for_each(std::execution::par_unseq, this->data_.begin(),
                  this->data_.end(),
                  [](std::vector<T> &x) { x.push_back(T(0.0)); });
    this->width_++;
  }

  /**
   * @brief Insert new empty unnamed column into dataframe
   */
  void InsertColumn() {
    if (this->has_header_row_) {
      throw HeaderStateMismatchException();
    }

    std::for_each(this->data_.begin(), this->data_.end(),
                  [](std::vector<T> &x) { x.push_back(T(0.0)); });
    this->width_++;
  }

  /**
   * @brief Get a Dataframe column in the form of a vector
   * @param index Position of the column to get
   * @return reference to a vector representing the column
   */
  std::vector<T> GetColumn(size_t index) const {
    std::vector<T> col{};

    for (size_t row = 0; row < this->height_; row++) {
      col.push_back(this->data_[row][index]);
    }

    return col;
  }

  /**
   * @brief Get a Dataframe column in the form of a vector
   * @param label Header of column to get
   * @return reference to a vector representing the column
   */
  std::vector<T> GetColumn(const std::string &label) const {
    // TODO
  }

  /**
   * @brief Get a table label given an index
   * @param index index of desired header
   * @return Referance to a string that holds the label
   */
  const std::string &GetHeader(size_t index) const {
    if (index >= this->width_) {
      throw DataframeIndexOutOfBoundsException();
    }
    return this->headers_[index];
  }

  /**
   * @brief Get any element in the dataframe with its row and column number
   * @param row Row number
   * @param column Column Number
   * @return Element stored at (row, column)
   */
  T GetElement(size_t row, size_t column) const {
    return this->data_[row][column];
  }

  /**
   * @brief Change an existing column given a vector
   * @param new_col New desired column
   * @param index position of old column to change
   */
  void SetColumn(const std::vector<T> &new_col, size_t index) {
    if (new_col.size() != this->height_) {
      throw DataframeVectorSizeMismatchException();
    }

    for (size_t row = 0; row < this->height_; row++) {
      this->data_[row][index] = new_col[row];
    }
  }

  void SetColumn(const std::vector<T> &new_col, const std::string &col_name) {
    // TODO
  }

  /**
   * @brief Create a dataframe with requested number of relavent dsp info
   * @param wave_table Dataframe of signals in a csv without headers
   * @param n Amount of data (Phases, amplitudes, and frequencies) desired
   * @return Pointer to a dataframe holding these data
   */
  std::unique_ptr<Dataframe<T>> GetDSPData(size_t n, size_t sample_rate) const {
    std::vector<std::string> headers{};

    for (size_t column = 1; column <= this->width_; column++) {
      for (size_t attribute = 0; attribute < 3; attribute++) {
        // TODO craft headers
        std::string tmp = "Signal ";
        tmp.append(std::to_string(column));

        std::string dsp_attrubute{};

        switch (attribute) {
        case 0:
          dsp_attrubute = " Frequencies";
          tmp.append(dsp_attrubute);
          break;
        case 1:
          dsp_attrubute = " Amplitudes";
          tmp.append(dsp_attrubute);
          break;
        case 2:
          dsp_attrubute = " Phases";
          tmp.append(dsp_attrubute);
          break;
        }

        headers.push_back(tmp);
      }
    }

    std::unique_ptr<Dataframe<T>> to_return(
        new Dataframe<T>(n, this->width_ * 3, headers));
    // result will have this->width*3 cols and n rows

    for (size_t column = 0; column < this->width_; column += 3) {
      std::vector<T> temporary_col = this->GetColumn(column);

      std::vector<T> tmp_frequencies{};
      std::vector<T> tmp_amplitudes{};
      std::vector<T> tmp_phases{};

      detail::ExtractNFrequencies(temporary_col, tmp_frequencies, sample_rate,
                                  n);
      detail::ExtractNAmplitudes(temporary_col, tmp_amplitudes, n);
      detail::ExtractNPhases(temporary_col, tmp_phases, n);

      to_return->RefactorColumn(column, tmp_frequencies);
      to_return->RefactorColumn(column + 1, tmp_amplitudes);
      to_return->RefactorColumn(column + 2, tmp_phases);
    }

    // TODO: Somehow change three above vectors to fit

    return to_return;
  }

  /**
   * @brief Write an existing dataframe to a CSV file
   * @param file_path File path to write the new file to. Must end in CSV
   */
  void ToCsv(const std::string &file_path) const {
    std::ofstream file;
    file.open(file_path);

    if (this->has_header_row_) {
      for (const std::string &header : this->headers_) {
        file << header;
        file << ",";
      }
    }

    file << "\n";

    for (const std::vector<T> &row : this->data_) {
      for (const T element : row) {
        file << element;
        file << ",";
      }
      file << "\n";
    }
  }

  void ToTxt(const std::string &file_path) {
    std::ofstream file;
    file.open(file_path);

    if (this->has_header_row_) {
      for (size_t i = 0;
           i < (this->max_column_width_ * this->width_) + (2 * padding); i++) {
        file << "_";
      }
      file << std::endl;
      for (const std::string &str : this->headers_) {
        file << std::setw(this->max_column_width_) << str << "|";
      }
      file << std::endl;
    }

    for (size_t i = 0;
         i < (this->max_column_width_ * this->width_) + (2 * padding); i++) {
      file << "_";
    }

    file << std::endl;

    for (size_t row = 0; row < this->height_; row++) {
      for (size_t col = 0; col < this->width_; col++) {
        file << std::setw(this->max_column_width_) << this->data_[row][col]
             << "|";
      }

      file << std::endl;
    }
  }

  /**
   * @brief Stack this's dataframe onto a given dataframe to concatenate the two
   * @param bottom Bottom frame to stack this on top of
   */
  void Stack(Dataframe<T> &bottom) {
    if (this->width_ != bottom.width_) {
      throw DataframeVectorSizeMismatchException();
    }

    if (!(this->headers_ == bottom.headers_)) {
      throw HeaderLabelMismatchException();
    }

    for (std::vector<T> &row : bottom.data_) {
      this->data_.push_back(row);
    }

    this->height_ += bottom.height_;
  }

  template <class V>
  friend inline std::ostream &operator<<(std::ostream &os,
                                         const Dataframe<V> &df);

  std::vector<T> operator[](size_t index) {
    return this->data_[index]; // get
  }

private:
  size_t height_;
  size_t width_;
  bool has_header_row_;
  std::vector<std::vector<T>> data_;
  std::vector<std::string> headers_;
  size_t max_column_width_;
}; // class Dataframe

template <class V>
inline std::ostream &operator<<(std::ostream &os, const Dataframe<V> &df) {
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
    for (const std::string &str : df.headers_) {
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

} // namespace read_file

#endif // CSVLIBRARY_READCSV_H
