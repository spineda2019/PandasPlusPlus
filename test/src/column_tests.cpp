#include "include/column_tests.hpp"

#include <memory>
#include <ppp/Column.hpp>
#include <vector>

namespace column_test {

namespace {
bool TestConstruction(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};

    ppp::Column col{data, "Key"};

    (*passes)++;

    return true;
}
}  // namespace

bool ColumnMasterTest(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
    return TestConstruction(passes, fails);
}

}  // namespace column_test
