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
#include <mutex>
#include <optional>
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

    Matrix<T>(std::size_t rows, std::size_t columns) noexcept
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

    Matrix(std::vector<std::vector<T>> data) noexcept
        : data_mutex_{},
          data_{data},
          headers_{std::nullopt},
          max_column_width_{15} {}

 private:
    std::mutex data_mutex_;
    std::vector<std::vector<T>> data_;
    std::optional<std::vector<std::string_view>> headers_;
    std::size_t max_column_width_;
};

}  // namespace ppp

#endif  // PPP_PPP_MATRIX_HPP
