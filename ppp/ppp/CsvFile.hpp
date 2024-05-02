/*
 *  CsvFile.hpp
 *  Lightweight abstraction for an fstream to a csv
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

#ifndef PPP_PPP_CSVFILE_HPP_
#define PPP_PPP_CSVFILE_HPP_

#include <optional>
#include <string>
namespace ppp {

class CsvFile {
 public:
    ~CsvFile() = default;
    constexpr CsvFile() {}

    [[nodiscard]]
    constexpr static std::optional<CsvFile> New(const std::string& path) {
        if (path.size() < 5) {
            return std::nullopt;
        } else {
            constexpr const char* csv{".csv"};
            return std::nullopt;
        }
    }

 private:
};

}  // namespace ppp

#endif  // PPP_PPP_CSVFILE_HPP_
