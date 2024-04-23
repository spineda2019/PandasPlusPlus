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
    Matrix()
        : data_mutex_{},
          data_{},
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
