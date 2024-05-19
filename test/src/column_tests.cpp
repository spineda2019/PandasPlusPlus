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
}  // namespace

bool ColumnMasterTest(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
    return TestConstruction(passes, fails);
}

}  // namespace column_test
