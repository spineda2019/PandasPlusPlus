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

#ifndef PPP_PPP_MATRIX_HPP_
#define PPP_PPP_MATRIX_HPP_

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <ostream>
#include <ranges>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

namespace ppp {
constexpr std::uint8_t padding{5};
template <class T>
concept AlgebraicTerm = requires(T value) {
    value == value;
    value = -value;
    value = value + T(1);
    value = value - T(1);
    std::cout << value;
};

template <AlgebraicTerm T>
class Matrix {
 public:
    ~Matrix() = default;

    Matrix(Matrix<T> &&to_move)
        : data_mutex_{}, data_{to_move.data_}, headers_{to_move.headers_} {}

    template <class... Args>
    static std::optional<Matrix<T>> New(Args &...args) noexcept {
        return Matrix<T>::FactoryHelper(std::forward<Args>(args)...);
    }

    [[nodiscard("You Must Check Success")]] std::optional<std::uint8_t>
    SetHeaders(const std::span<std::string_view> headers) {
        if (std::ranges::size(headers) != data_[0].size()) {
            return std::nullopt;
        } else {
            std::vector<std::string_view> tmp_headers{};
            tmp_headers.reserve(data_[0].size());
            for (const std::string_view header : headers) {
                tmp_headers.emplace_back(header);
            }
            headers_ = std::move(tmp_headers);
            return 0;
        }
    }

    template <AlgebraicTerm V>
    friend inline std::ostream &operator<<(std::ostream &stream,
                                           const Matrix<V> &matrix);

    template <AlgebraicTerm V>
    friend inline std::optional<Matrix<V>> operator+(const Matrix<V> &lhs,
                                                     const Matrix<V> &rhs);

 private:
    Matrix() noexcept : data_mutex_{}, data_{}, headers_{std::nullopt} {}

    Matrix(std::size_t rows, std::size_t columns) noexcept
        : data_mutex_{},
          data_{std::vector<std::vector<T>>(rows,
                                            std::vector<T>(columns, T(0.0)))},
          headers_{std::nullopt} {}

    explicit Matrix(std::vector<std::vector<T>> &&data) noexcept
        : data_mutex_{}, data_{data}, headers_{std::nullopt} {}

    explicit Matrix(std::vector<std::vector<T>> &data) noexcept
        : data_mutex_{}, data_{data}, headers_{std::nullopt} {}

    static std::optional<Matrix<T>> FactoryHelper() noexcept {
        return std::make_optional<Matrix<T>>(Matrix<T>{});
    }

    static std::optional<Matrix<T>> FactoryHelper(
        std::size_t rows, std::size_t columns) noexcept {
        return std::make_optional<Matrix<T>>(Matrix<T>{rows, columns});
    }

    static std::optional<Matrix<T>> FactoryHelper(
        std::vector<std::vector<T>> &&data) noexcept {
        if (data.size() == 0) {
            return std::nullopt;
        } else if (std::size_t width{data[0].size()}; width == 0) {
            return std::nullopt;
        } else {
            for (const std::vector<T> &row : data) {
                if (row.size() != width) {
                    return std::nullopt;
                }
            }

            return std::make_optional<Matrix<T>>(Matrix<T>{data});
        }
    }

    static std::optional<Matrix<T>> FactoryHelper(
        const std::vector<std::vector<T>> &data) noexcept {
        if (data.size() == 0) {
            return std::nullopt;
        } else if (std::size_t width{data[0].size()}; width == 0) {
            return std::nullopt;
        } else {
            for (const std::vector<T> &row : data) {
                if (row.size() != width) {
                    return std::nullopt;
                }
            }

            return std::make_optional<Matrix<T>>(Matrix<T>{data});
        }
    }

 private:
    std::mutex data_mutex_;
    std::vector<std::vector<T>> data_;
    std::optional<std::vector<std::string_view>> headers_;
    static constexpr std::size_t MAX_COLUMN_WIDTH{15};
};  // class Matrix

template <AlgebraicTerm V>
inline std::ostream &operator<<(std::ostream &stream, const Matrix<V> &matrix) {
    constexpr std::uint8_t max_height{20};
    std::size_t rows_to_print{};

    if (std::size_t height = matrix.data_.size(); height == 0) {
        return stream;
    } else if (height >= max_height) {
        rows_to_print = max_height / 2;
    } else {
        rows_to_print = height / 2;
    }

    const std::size_t max_table_width{
        (matrix.data_.size() * matrix.data_[0].size()) + (2 * padding)};

    if (matrix.headers_.has_value()) {
        for (std::size_t i{0}; i < max_table_width; i++) {
            stream << "_";
        }
        stream << std::endl;
        for (const std::string_view &header : matrix.headers_.value()) {
            stream << std::setw(Matrix<V>::MAX_COLUMN_WIDTH) << header << "|";
        }
        stream << std::endl;
    }

    for (std::size_t i = 0; i < max_table_width; i++) {
        stream << "_";
    }
    stream << std::endl;

    for (const std::vector<V> &row : matrix.data_) {
        for (const V &element : row) {
            stream << std::setw(Matrix<V>::MAX_COLUMN_WIDTH) << element << "|";
        }
    }

    if (rows_to_print > 1) {
        stream << std::endl;
        stream << std::setw(
                      (Matrix<V>::MAX_COLUMN_WIDTH * matrix.data_[0].size()) /
                      2)
               << "...";
        stream << std::endl << std::endl;

        // bottom
        for (const std::vector<V> &row : matrix.data_) {
            for (const V &element : row) {
                stream << std::setw(Matrix<V>::MAX_COLUMN_WIDTH) << element
                       << "|";
            }
            stream << std::endl;
        }
    }

    return stream;
}

template <AlgebraicTerm V>
inline std::optional<Matrix<V>> operator+(const Matrix<V> &lhs,
                                          const Matrix<V> &rhs) {
    if (lhs.data_.size() != rhs.data_.size()) {
        return std::nullopt;
    } else if (lhs.data_[0].size() != rhs.data_[0].size()) {
        return std::nullopt;
    } else {
        std::vector<std::vector<V>> new_data{
            lhs.data_.size(), std::vector<V>(lhs.data_[0].size())};

        auto sum = std::views::zip_transform(
            [](const auto &left_row, const auto &right_row) {
                return std::views::zip_transform(
                    [](const auto &left_val, const auto &right_val) {
                        return left_val + right_val;
                    },
                    left_row, right_row);
            },
            lhs.data_, rhs.data_);

        for (const auto &[summed_row, new_row] :
             std::views::zip(sum, new_data)) {
            for (const auto &[summed_val, new_val] :
                 std::views::zip(summed_row, new_row)) {
                new_val = summed_val;
            }
            std::cout << std::endl;
        }

        return std::make_optional<Matrix<V>>(Matrix<V>{new_data});
    }
}
}  // namespace ppp

#endif  // PPP_PPP_MATRIX_HPP_
