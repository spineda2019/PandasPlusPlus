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

#include <string>
#include <string_view>
#include <vector>
namespace ppp {

template <class T>
concept BasicEntry = requires(T first, T second) {
    first + second;
    first - second;
    first / second;
};

template <BasicEntry T>
class Column {
 public:
    Column(const std::vector<T>& data, std::string_view key)
        : data_{data}, key_{key} {}

 private:
    std::vector<T> data_;
    std::string key_;
};

}  // namespace ppp

#endif  // PPP_PPP_COLUMN_HPP_
