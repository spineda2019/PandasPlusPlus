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

#include <algorithm>
#include <complex>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <ostream>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>
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

template <class T>
concept IsComplex = requires(T value) {
    T::value_type;
    std::is_same_v<T, std::complex<typename T::value_type>> == true;
};

template <class T>
concept SimpleNumber =
    std::integral<T> || std::floating_point<T> || IsComplex<T>;

template <AlgebraicTerm T>
class Matrix {
 public:
    ~Matrix() = default;

    Matrix(Matrix<T> &&to_move)
        : data_mutex_{},
          data_{to_move.data_},
          headers_{to_move.headers_},
          height_{to_move.height_},
          width_{to_move.width_} {}

    template <class... Args>
    static std::optional<Matrix<T>> New(Args... args) noexcept {
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
                                           const Matrix<V> &matrix) noexcept;

    template <AlgebraicTerm V>
    friend inline std::optional<Matrix<V>> operator+(
        const Matrix<V> &lhs, const Matrix<V> &rhs) noexcept;

    template <AlgebraicTerm V>
    friend inline std::optional<Matrix<V>> operator-(
        const Matrix<V> &lhs, const Matrix<V> &rhs) noexcept;

    template <AlgebraicTerm V, SimpleNumber U>
    friend inline std::optional<Matrix<V>> operator-(const Matrix<V> &lhs,
                                                     U rhs) noexcept;

    template <AlgebraicTerm V, SimpleNumber U>
    friend inline std::optional<Matrix<V>> operator+(const Matrix<V> &lhs,
                                                     U rhs) noexcept;

    template <AlgebraicTerm V>
    friend inline bool operator==(const Matrix<V> &lhs,
                                  const Matrix<V> &rhs) noexcept;

 private:
    Matrix() noexcept
        : data_mutex_{},
          data_{},
          headers_{std::nullopt},
          height_{0},
          width_{0} {}

    Matrix(std::size_t rows, std::size_t columns) noexcept
        : data_mutex_{},
          data_{std::vector<std::vector<T>>(rows,
                                            std::vector<T>(columns, T(0.0)))},
          headers_{std::nullopt},
          height_{rows},
          width_{columns} {}

    explicit Matrix(std::vector<std::vector<T>> &&data) noexcept
        : data_mutex_{},
          data_{data},
          headers_{std::nullopt},
          height_{data.size()},
          width_{data[0].size()} {}

    explicit Matrix(std::vector<std::vector<T>> &data) noexcept
        : data_mutex_{},
          data_{data},
          headers_{std::nullopt},
          height_{data.size()},
          width_{data[0].size()} {}

    template <SimpleNumber U>
    Matrix(std::size_t rows, std::size_t columns, U value) noexcept
        : data_mutex_{},
          data_{std::vector<std::vector<U>>(rows,
                                            std::vector<U>(columns, value))},
          headers_{std::nullopt},
          height_{rows},
          width_{columns} {}

    static std::optional<Matrix<T>> FactoryHelper() noexcept {
        return std::make_optional<Matrix<T>>(Matrix<T>{});
    }

    static std::optional<Matrix<T>> FactoryHelper(
        std::size_t rows, std::size_t columns) noexcept {
        // Prevent matrices that don't make physical sense!
        if (rows == 0 && columns != 0) {
            return std::nullopt;
        } else {
            return std::make_optional<Matrix<T>>(Matrix<T>{rows, columns});
        }
    }

    static std::optional<Matrix<T>> FactoryHelper(
        std::vector<std::vector<T>> &&data) noexcept {
        if (data.size() == 0) {
            return std::make_optional<Matrix<T>>(Matrix<T>{});
        } else {
            std::size_t width{data[0].size()};
            for (const std::vector<T> &row : data) {
                if (row.size() != width) {
                    return std::nullopt;
                }
            }

            return std::make_optional<Matrix<T>>(Matrix<T>{std::move(data)});
        }
    }

    static std::optional<Matrix<T>> FactoryHelper(
        const std::vector<std::vector<T>> &data) noexcept {
        if (data.size() == 0) {
            return std::make_optional<Matrix<T>>(Matrix<T>{});
        } else {
            std::size_t width{data[0].size()};
            for (const std::vector<T> &row : data) {
                if (row.size() != width) {
                    return std::nullopt;
                }
            }

            return std::make_optional<Matrix<T>>(Matrix<T>{data});
        }
    }

    template <SimpleNumber U>
    static std::optional<Matrix<T>> FactoryHelper(std::size_t rows,
                                                  std::size_t columns,
                                                  U val) noexcept {
        if (rows == 0) {
            return std::make_optional<Matrix<T>>(Matrix<T>{});
        } else if (columns == 0) {
            return std::make_optional<Matrix<T>>(Matrix<T>{rows, columns});
        } else {
            return std::make_optional<Matrix<T>>(Matrix<T>{rows, columns, val});
        }
    }

 private:
    std::mutex data_mutex_;
    std::size_t height_;
    std::size_t width_;
    std::vector<std::vector<T>> data_;
    std::optional<std::vector<std::string_view>> headers_;
    static constexpr std::size_t MAX_COLUMN_WIDTH{15};
};  // class Matrix

template <AlgebraicTerm V>
inline std::ostream &operator<<(std::ostream &stream,
                                const Matrix<V> &matrix) noexcept {
    constexpr std::uint8_t max_height{20};
    std::size_t rows_to_print{};

    if (matrix.height_ == 0) {
        return stream;
    } else if (matrix.height_ >= max_height) {
        rows_to_print = max_height / 2;
    } else {
        rows_to_print = matrix.height_ / 2;
    }

    const std::size_t max_table_width{(matrix.height_ * matrix.width_) +
                                      (2 * padding)};

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
        stream << std::endl;
    }

    return stream;
}

template <AlgebraicTerm V>
inline std::optional<Matrix<V>> operator+(const Matrix<V> &lhs,
                                          const Matrix<V> &rhs) noexcept {
    if ((lhs.height_ != rhs.height_) || (lhs.width_ != rhs.width_)) {
        return std::nullopt;
    } else {
        std::vector<std::vector<V>> new_data{lhs.height_,
                                             std::vector<V>(lhs.width_)};

        std::transform(std::execution::par_unseq, lhs.data_.begin(),
                       lhs.data_.end(), rhs.data_.begin(), new_data.begin(),
                       [](const std::vector<V> &left_row,
                          const std::vector<V> &right_row) {
                           std::vector<V> row(left_row.size());
                           std::transform(left_row.begin(), left_row.end(),
                                          right_row.begin(), row.begin(),
                                          [](const V &left, const V &right) {
                                              return left + right;
                                          });
                           return std::move(row);
                       });

        return std::make_optional<Matrix<V>>(Matrix<V>{new_data});
    }
}

template <AlgebraicTerm V>
inline std::optional<Matrix<V>> operator-(const Matrix<V> &lhs,
                                          const Matrix<V> &rhs) noexcept {
    if ((lhs.height_ != rhs.height_) || (lhs.width_ != rhs.width_)) {
        return std::nullopt;
    } else {
        std::vector<std::vector<V>> new_data{lhs.height_,
                                             std::vector<V>(lhs.width_)};

        std::transform(std::execution::par_unseq, lhs.data_.begin(),
                       lhs.data_.end(), rhs.data_.begin(), new_data.begin(),
                       [](const std::vector<V> &left_row,
                          const std::vector<V> &right_row) {
                           std::vector<V> row(left_row.size());
                           std::transform(left_row.begin(), left_row.end(),
                                          right_row.begin(), row.begin(),
                                          [](const V &left, const V &right) {
                                              return left - right;
                                          });
                           return std::move(row);
                       });

        return std::make_optional<Matrix<V>>(Matrix<V>{new_data});
    }
}
template <AlgebraicTerm V, SimpleNumber U>
inline std::optional<Matrix<V>> operator-(const Matrix<V> &lhs,
                                          U rhs) noexcept {
    const std::size_t height{lhs.height_};
    const std::size_t width{lhs.width_};

    const std::optional<Matrix<V>> right{Matrix<V>::New(height, width, rhs)};

    if (right.has_value()) {
        return lhs - right.value();
    } else {
        return std::nullopt;
    }
}

template <AlgebraicTerm V, SimpleNumber U>
inline std::optional<Matrix<V>> operator+(const Matrix<V> &lhs,
                                          U rhs) noexcept {
    const std::size_t height{lhs.height_};
    const std::size_t width{lhs.width_};

    const std::optional<Matrix<V>> right{Matrix<V>::New(height, width, rhs)};

    if (right.has_value()) {
        return lhs + right.value();
    } else {
        return std::nullopt;
    }
}

template <AlgebraicTerm V>
inline bool operator==(const Matrix<V> &lhs, const Matrix<V> &rhs) noexcept {
    if ((lhs.height_ != rhs.height_) || (lhs.width_ != rhs.width_)) {
        return false;
    } else {
        return !(
            std::ranges::find_if(
                std::views::zip(lhs.data_, rhs.data_),
                [](std::pair<std::span<const V>, std::span<const V>> rows) {
                    return std::ranges::find_if(
                               std::views::zip(rows.first, rows.second),
                               [](std::pair<const V, const V> vals) {
                                   return vals.first != vals.second;
                               }) !=
                           std::ranges::end(
                               std::views::zip(rows.first, rows.second));
                }) != std::ranges::end(std::views::zip(lhs.data_, rhs.data_)));
    }
}

}  // namespace ppp

#endif  // PPP_PPP_MATRIX_HPP_
