#ifndef TEST_SRC_INCLUDE_COLUMN_TESTS_HPP_
#define TEST_SRC_INCLUDE_COLUMN_TESTS_HPP_

#include <cstddef>
#include <memory>

namespace column_test {

bool ColumnMasterTest(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails);

}

#endif  // TEST_SRC_INCLUDE_COLUMN_TESTS_HPP_
