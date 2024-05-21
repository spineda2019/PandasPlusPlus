#include "include/column_tests.hpp"

#include <memory>
#include <string_view>
#include <vector>

#include "ppp/Column.hpp"

namespace column_test {

namespace {

template <ppp::BasicEntry T>
inline void PassNotification(const ppp::Column<T>& col,
                             const std::string_view test_name) {
    std::cout << test_name << " Passed! Column Used:" << std::endl << col;
}

template <ppp::BasicEntry T>
inline void FailNotification(const ppp::Column<T>& col,
                             const std::string_view test_name) {
    std::cout << test_name << " Failed... Column Used:" << std::endl << col;
}

bool TestConstruction(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};

    ppp::Column col{data, "Key"};

    (*passes)++;

    PassNotification(col, "TestConstruction");

    return true;
}

bool TestAddition(const std::unique_ptr<std::size_t>& passes,
                  const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};
    std::vector<int> short_data{1, 5};

    ppp::Column col{data, "Key"};
    ppp::Column short_col{short_data, "Key"};

    auto bad_sum = col + short_col;
    if (bad_sum.has_value()) {
        FailNotification(col, "TestAddition");
        (*fails)++;
        return false;
    }

    auto sum = col + col;

    if (!sum.has_value()) {
        FailNotification(col, "TestAddition");
        (*fails)++;
        return false;
    } else {
        PassNotification(sum.value(), "TestAddition");
        (*passes)++;
        return true;
    }
}
}  // namespace

bool ColumnMasterTest(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
    return TestConstruction(passes, fails) && TestAddition(passes, fails);
}

}  // namespace column_test
