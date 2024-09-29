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
#include <complex>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
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
    std::abs(first);
    T(0);
    std::cout << first;
};

template <class T>
concept SimpleNumber = std::integral<T> || std::floating_point<T>;

template <class T>
concept SimpleComplexNumber = requires(T value) {
    T::value_type;
    requires SimpleNumber<typename T::value_type>;
    requires std::same_as<T, std::complex<typename T::value_type>>;
};

template <class T>
concept Number = SimpleComplexNumber<T> || SimpleNumber<T>;

template <BasicEntry T>
class Column {
 public:
    constexpr Column(const std::vector<T> &data, const std::string_view key)
        : data_{data}, key_{key} {}

    constexpr Column(const std::vector<T> &&data, const std::string_view key)
        : data_{data}, key_{key} {}

    constexpr explicit Column(const Column<T> &&moved)
        : data_{moved.data_}, key_{moved.key_} {}

    constexpr inline T Sum() {
        return std::reduce(std::execution::par_unseq, data_.begin(),
                           data_.end());
    }

    constexpr std::size_t Size() const { return data_.size(); }

    constexpr T LNorm(std::optional<std::size_t> norm) const {
        if (!norm.has_value()) {
            // L-Infinity - max element magnitude

            return std::abs(*std::max_element(
                std::execution::par_unseq, data_.cbegin(), data_.cend(),
                [](const T &left, const T &right) {
                    return std::abs(left) < std::abs(right);
                }));

        } else if (std::size_t norm_val{norm.value()}; norm_val == 1) {
            // L-1 - Manhattan Distance
            return std::reduce(
                std::execution::par_unseq, data_.cbegin(), data_.cend(), T(0),
                [](const T &lhs, const T &rhs) { return lhs + std::abs(rhs); });
        } else if (norm_val == 0) {
            // L-0 - Number of non 0s
            return T(std::count_if(
                std::execution::par_unseq, data_.cbegin(), data_.cend(),
                [](const T &value) { return value != T(0); }));
        } else {
            return std::pow(
                std::reduce(std::execution::par_unseq, data_.cbegin(),
                            data_.cend(), T(0),
                            [&norm_val](const T &lhs, const T &rhs) {
                                return lhs + std::pow(std::abs(rhs), norm_val);
                            }),
                (1.0 / static_cast<double>(norm_val)));
        }
    }

    constexpr std::optional<T> Dot(const Column<T> &rhs) const {
        if (rhs.Size() != this->Size()) {
            return std::nullopt;
        } else {
            auto it = std::ranges::views::zip(this->data_, rhs.data_);
            std::atomic<T> sum{};
            std::for_each(std::execution::par_unseq, it.begin(), it.end(),
                          [&sum](const std::pair<T, T> &x) {
                              sum += x.first * x.second;
                          });
            return sum;
        }
    }

    constexpr std::optional<Column<T>> Cross3D(const Column<T> &rhs,
                                               std::string &&key = "") const {
        if (rhs.data_.size() != 3 || this->data_.size() != 3) {
            return std::nullopt;
        } else {
            std::vector<T> cross{
                (data_[1] * rhs.data_[2]) - (data_[2] * rhs.data_[1]),
                (data_[2] * rhs.data_[0]) - (data_[0] * rhs.data_[2]),
                (data_[0] * rhs.data_[1]) - (data_[1] * rhs.data_[0]),
            };

            return std::make_optional<Column<T>>(std::move(cross),
                                                 std::move(key));
        }
    }

    constexpr void Append(T &&value) { data_.emplace_back(value); }
    constexpr void Append(const T &value) { data_.emplace_back(value); }

    /* ********************************************************************** */
    /*                               Operators                                */
    /* ********************************************************************** */

    constexpr inline std::optional<T> operator[](std::size_t index) {
        if (index >= data_.size()) {
            return std::nullopt;
        } else {
            return data_[index];
        }
    }

    /* ********************************************************************** */
    /*                                Friends                                 */
    /* ********************************************************************** */

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
    constexpr friend inline std::optional<V> operator*(const Column<V> &lhs,
                                                       const Column<V> &rhs);

    template <Number V>
    constexpr friend inline Column<V> operator*(const V &lhs,
                                                const Column<V> &rhs);

    template <Number V>
    constexpr friend inline Column<V> operator*(const Column<V> &lhs,
                                                const V &rhs);

    template <BasicEntry V>
    constexpr friend inline bool operator==(const Column<V> &lhs,
                                            const Column<V> &rhs);

 private:
    std::vector<T> data_;
    std::string key_;
};

template <BasicEntry V>
inline std::ostream &operator<<(std::ostream &stream, const Column<V> &column) {
    stream << "\"" << column.key_ << "\""
           << " | ";
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

template <BasicEntry V>
constexpr inline std::optional<V> operator*(const Column<V> &lhs,
                                            const Column<V> &rhs) {
    return lhs.Dot(rhs);
}

template <Number V>
constexpr inline Column<V> operator*(const V &lhs, const Column<V> &rhs) {
    std::vector<V> data(rhs.data_.size());

    std::transform(std::execution::par_unseq, rhs.data_.cbegin(),
                   rhs.data_.cend(), data.begin(),
                   [&lhs](const V &entry) { return lhs * entry; });

    return Column(std::move(data), rhs.key_ + " * " + std::to_string(lhs));
}

template <Number V>
constexpr inline Column<V> operator*(const Column<V> &lhs, const V &rhs) {
    return rhs * lhs;
}

}  // namespace ppp

#endif  // PPP_PPP_COLUMN_HPP_
