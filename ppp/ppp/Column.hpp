/*
 *  Column.hpp
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

#ifndef PPP_PPP_COLUMN_HPP_
#define PPP_PPP_COLUMN_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace ppp {

enum class ErrorCode : std::uint8_t {
    MismatchedColumnSize,
};

template <class T>
concept BasicEntry = requires(T first, T second) {
    first + second;
    first - second;
    first / second;
    (first * second);
    std::cout << first;
};

template <BasicEntry T>
class Column {
 public:
    constexpr Column(const std::vector<T> &data, std::string_view key)
        : data_{data}, key_{key} {}

    constexpr Column(const std::vector<T> &&data, std::string_view key)
        : data_{data}, key_{key} {}

    constexpr explicit Column(const Column<T> &&moved)
        : data_{moved.data_}, key_{moved.key_} {}

    constexpr inline T Sum() {
        return std::reduce(std::execution::par_unseq, data_.begin(),
                           data_.end());
    }

    template <BasicEntry V>
    friend inline std::ostream &operator<<(std::ostream &stream,
                                           const Column<V> &column);

    template <BasicEntry V>
    constexpr friend inline std::optional<Column<V>> operator+(
        const Column<V> &lhs, const Column<V> &rhs);

    template <BasicEntry V>
    constexpr friend inline std::optional<Column<V>> operator-(
        const Column<V> &lhs, const Column<V> &rhs);

    template <BasicEntry V>
    constexpr friend inline bool operator==(const Column<V> &lhs,
                                            const Column<V> &rhs);

    constexpr inline std::optional<T> operator[](std::size_t index) {
        if (index >= data_.size()) {
            return std::nullopt;
        } else {
            return data_[index];
        }
    }

 private:
    std::vector<T> data_;
    std::string key_;
};

template <BasicEntry V>
inline std::ostream &operator<<(std::ostream &stream, const Column<V> &column) {
    stream << "\"" << column.key_ << "\"" << " | ";
    for (const V &entry : column.data_) {
        stream << entry << " | ";
    }
    stream << std::endl;
    return stream;
}

template <BasicEntry V>
constexpr inline std::optional<Column<V>> operator+(const Column<V> &lhs,
                                                    const Column<V> &rhs) {
    if (lhs.data_.size() != rhs.data_.size()) {
        return std::nullopt;
    } else {
        std::vector<V> sum(lhs.data_.size());
        std::transform(std::execution::par_unseq, lhs.data_.cbegin(),
                       lhs.data_.cend(), rhs.data_.cbegin(), sum.begin(),
                       std::plus<V>());

        return std::make_optional<Column<V>>(std::move(sum),
                                             lhs.key_ + " + " + rhs.key_);
    }
}

template <BasicEntry V>
constexpr inline std::optional<Column<V>> operator-(const Column<V> &lhs,
                                                    const Column<V> &rhs) {
    if (lhs.data_.size() != rhs.data_.size()) {
        return std::nullopt;
    } else {
        std::vector<V> diff(lhs.data_.size());
        std::transform(std::execution::par_unseq, lhs.data_.cbegin(),
                       lhs.data_.cend(), rhs.data_.cbegin(), diff.begin(),
                       std::minus<V>());

        return std::make_optional<Column<V>>(std::move(diff),
                                             lhs.key_ + " - " + rhs.key_);
    }
}

template <BasicEntry V>
constexpr inline bool operator==(const Column<V> &lhs, const Column<V> &rhs) {
    return lhs.data_ == rhs.data_;
}

}  // namespace ppp

#endif  // PPP_PPP_COLUMN_HPP_
