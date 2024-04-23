/*
 *  Matrix.hpp
 *  Datatype class for use with common linear algebra operations
 *
 *  Copyright (C) 2024 Sebastian Pineda (spineda.wpi.alum@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License Version 3.0 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License and GNU Lesser General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PPP_PPP_MATRIX_HPP
#define PPP_PPP_MATRIX_HPP

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <ostream>
#include <string_view>
#include <vector>

namespace ppp {
constexpr std::uint8_t padding{5};
template <class T>
concept AlgebraicTerm = requires(T value) {
    value == value;
    value = -value;
    value = value + 1;
    value = value - 1;
    std::cout << value;
};

template <AlgebraicTerm T>
class Matrix {
 public:
    ~Matrix() = default;
    Matrix() noexcept
        : data_mutex_{},
          data_{},
          headers_{std::nullopt},
          max_column_width_{15} {}

    Matrix(std::size_t rows, std::size_t columns) noexcept
        : data_mutex_{},
          data_{std::vector<std::vector<T>>(rows,
                                            std::vector<T>(columns, T(0.0)))},
          headers_{std::nullopt},
          max_column_width_{15} {}

    Matrix(std::vector<std::vector<T>> &&data) noexcept
        : data_mutex_{},
          data_{data},
          headers_{std::nullopt},
          max_column_width_{15} {}

    Matrix(std::vector<std::vector<T>> &data) noexcept
        : data_mutex_{},
          data_{data},
          headers_{std::nullopt},
          max_column_width_{15} {}

    template <AlgebraicTerm V>
    friend inline std::ostream &operator<<(std::ostream &stream,
                                           const Matrix<V> &matrix);

 private:
    std::mutex data_mutex_;
    std::vector<std::vector<T>> data_;
    std::optional<std::vector<std::string_view>> headers_;
    std::size_t max_column_width_;
};  // class Matrix

template <AlgebraicTerm V>
inline std::ostream &operator<<(std::ostream &stream, const Matrix<V> &matrix) {
    constexpr std::uint8_t max_height{20};
    const std::size_t max_table_width{
        (matrix.data_.size() * matrix.data_[0].size()) + (2 * padding)};
    std::size_t rows_to_print{};

    if (std::size_t height = matrix.data_.size(); height >= max_height) {
        rows_to_print = max_height / 2;
    } else {
        rows_to_print = height / 2;
    }

    if (matrix.headers_.has_value()) {
        for (std::size_t i{0}; i < max_table_width; i++) {
            stream << "_";
        }
        stream << std::endl;
        for (const std::string_view &header : matrix.headers_.value()) {
            stream << std::setw(matrix.max_column_width_) << header << "|";
        }
        stream << std::endl;
    }

    for (std::size_t i = 0; i < max_table_width; i++) {
        stream << "_";
    }
    stream << std::endl;

    for (const std::vector<V> &row : matrix.data_) {
        for (const V &element : row) {
            stream << std::setw(matrix.max_column_width_) << element << "|";
        }
    }

    if (rows_to_print > 1) {
        stream << std::endl;
        stream << std::setw(
                      (matrix.max_column_width_ * matrix.data_[0].size()) / 2)
               << "...";
        stream << std::endl << std::endl;

        // bottom
        for (const std::vector<V> &row : matrix.data_) {
            for (const V &element : row) {
                stream << std::setw(matrix.max_column_width_) << element << "|";
            }
            stream << std::endl;
        }
    }

    return stream;
}
}  // namespace ppp

#endif  // PPP_PPP_MATRIX_HPP
