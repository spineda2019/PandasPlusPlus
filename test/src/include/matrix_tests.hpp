#ifndef TEST_SRC_INCLUDE_MATRIX_TESTS_HPP_
#define TEST_SRC_INCLUDE_MATRIX_TESTS_HPP_

#include <cstddef>
#include <memory>
namespace matrix_test {
bool MatrixMasterTest(const std::unique_ptr<std::size_t> &passes,
                      const std::unique_ptr<std::size_t> &fails);

bool TestEquality(const std::unique_ptr<std::size_t> &passes,
                  const std::unique_ptr<std::size_t> &fails);
bool TestHeadlessPrint(const std::unique_ptr<std::size_t> &passes,
                       const std::unique_ptr<std::size_t> &fails);
bool TestEmptyPrint(const std::unique_ptr<std::size_t> &passes,
                    const std::unique_ptr<std::size_t> &fails);
bool TestComplexPrint(const std::unique_ptr<std::size_t> &passes,
                      const std::unique_ptr<std::size_t> &fails);
bool TestInsertingHeaders(const std::unique_ptr<std::size_t> &passes,
                          const std::unique_ptr<std::size_t> &fails);
bool TestBadShapeCatching(const std::unique_ptr<std::size_t> &passes,
                          const std::unique_ptr<std::size_t> &fails);
bool TestAddition(const std::unique_ptr<std::size_t> &passes,
                  const std::unique_ptr<std::size_t> &fails);
bool TestSubtraction(const std::unique_ptr<std::size_t> &passes,
                     const std::unique_ptr<std::size_t> &fails);
bool TestNonMatrixSubtraction(const std::unique_ptr<std::size_t> &passes,
                              const std::unique_ptr<std::size_t> &fails);
}  // namespace matrix_test

#endif  // TEST_SRC_INCLUDE_MATRIX_TESTS_HPP_
