#ifndef TEST_SRC_INCLUDE_MATRIX_TESTS_HPP_
#define TEST_SRC_INCLUDE_MATRIX_TESTS_HPP_

#include <cstddef>
#include <memory>
namespace matrix_test {
bool MatrixMasterTest(const std::unique_ptr<std::size_t> &passes,
                      const std::unique_ptr<std::size_t> &fails);
}  // namespace matrix_test

#endif  // TEST_SRC_INCLUDE_MATRIX_TESTS_HPP_
