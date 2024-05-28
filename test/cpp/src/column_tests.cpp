#include "include/column_tests.hpp"

#include <complex>
#include <memory>
#include <optional>
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

bool TestDot(const std::unique_ptr<std::size_t>& passes,
             const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};
    std::vector<int> short_data{1, 5};

    ppp::Column col{data, "Key"};
    ppp::Column short_col{short_data, "Key"};

    std::optional<int> dot = col * short_col;

    if (dot.has_value()) {
        FailNotification(col, "TestDot");
        (*fails)++;
        return false;
    }

    std::optional<int> good_dot = col * col;

    if (!good_dot.has_value()) {
        FailNotification(col, "TestDot");
        (*fails)++;
        return false;
    } else if (good_dot.value() != 62) {
        FailNotification(col, "TestDot");
        (*fails)++;
        return false;
    } else {
        PassNotification(col, "TestDot");
        (*passes)++;
        return true;
    }
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

    std::vector<int> data2{2, 10, 12};
    ppp::Column col2{data2, "Key"};

    if (!sum.has_value()) {
        FailNotification(col, "TestAddition");
        (*fails)++;
        return false;
    } else if (sum.value() != col2) {
        FailNotification(col, "TestAddition");
        (*fails)++;
        return false;
    } else {
        PassNotification(sum.value(), "TestAddition");
        (*passes)++;
        return true;
    }
}

bool TestSubtraction(const std::unique_ptr<std::size_t>& passes,
                     const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};
    std::vector<int> short_data{1, 5};

    ppp::Column col{data, "Key"};
    ppp::Column short_col{short_data, "Key"};

    auto bad_sum = col - short_col;
    if (bad_sum.has_value()) {
        FailNotification(col, "TestAddition");
        (*fails)++;
        return false;
    }

    auto sum = col - col;

    std::vector<int> data2{0, 0, 0};
    ppp::Column col2{data2, "Key"};

    if (!sum.has_value()) {
        FailNotification(col, "TestAddition");
        (*fails)++;
        return false;
    } else if (sum.value() != col2) {
        FailNotification(col, "TestAddition");
        (*fails)++;
        return false;
    } else {
        PassNotification(sum.value(), "TestAddition");
        (*passes)++;
        return true;
    }
}

bool TestComparison(const std::unique_ptr<std::size_t>& passes,
                    const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};
    std::vector<int> data2{1, 5, 6};
    std::vector<int> short_data{1, 5};

    ppp::Column col{data, "Key"};
    ppp::Column col2{data2, "Key"};
    ppp::Column short_col{short_data, "Key"};

    if (col == short_col) {
        FailNotification(col, "TestComparison");
        (*fails)++;
        return false;
    } else if (col != col2) {
        FailNotification(col, "TestComparison");
        (*fails)++;
        return false;
    } else {
        PassNotification(col, "TestComparison");
        (*passes)++;
        return true;
    }
}

bool TestIndexing(const std::unique_ptr<std::size_t>& passes,
                  const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};
    ppp::Column col{data, "Key"};

    std::optional<int> value{col[0]};

    if (!value.has_value()) {
        FailNotification(col, "TestIndexing");
        (*fails)++;
        return false;
    } else if (value.value() != 1) {
        FailNotification(col, "TestIndexing");
        (*fails)++;
        return false;
    }

    value = col[4];

    if (value.has_value()) {
        FailNotification(col, "TestIndexing");
        (*fails)++;
        return false;
    }

    PassNotification(col, "TestIndexing");
    (*passes)++;
    return true;
}

bool TestSum(const std::unique_ptr<std::size_t>& passes,
             const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};
    ppp::Column col{data, "Key"};

    if (col.Sum() == 12) {
        PassNotification(col, "TestSum");
        (*passes)++;
        return true;
    } else {
        FailNotification(col, "TestSum");
        (*fails)++;
        return false;
    }
}

bool TestAppend(const std::unique_ptr<std::size_t>& passes,
                const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};
    std::vector<int> data2{1, 5, 6, 7};

    ppp::Column col{data, "Key"};
    ppp::Column col2{data2, "Key"};

    col.Append(7);

    if (col == col2) {
        PassNotification(col, "TestAppend");
        (*passes)++;
        return true;
    } else {
        FailNotification(col, "TestAppend");
        (*fails)++;
        return false;
    }
}

bool TestScale(const std::unique_ptr<std::size_t>& passes,
               const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{1, 5, 6};
    std::vector<int> scaled_data{3, 15, 18};

    ppp::Column col{data, "Key"};
    ppp::Column scaled_col{scaled_data, "Key"};

    ppp::Column scaled = 3 * col;

    if (scaled != scaled_col) {
        FailNotification(col, "TestScale");
        (*fails)++;
        return false;
    }

    ppp::Column scaled_again = col * 3;

    if (scaled_again != scaled_col) {
        FailNotification(col, "TestScale");
        (*fails)++;
        return false;
    } else {
        PassNotification(scaled_again, "TestScale");
        (*passes)++;
        return true;
    }
}

bool TestNorm(const std::unique_ptr<std::size_t>& passes,
              const std::unique_ptr<std::size_t>& fails) {
    std::vector<int> data{-3, -4};
    ppp::Column col{data, "Key"};

    if (col.LNorm(1) != 7) {
        FailNotification(col, "TestNorm");
        (*fails)++;
        std::cout << "L-1 Failed" << col.LNorm(1) << std::endl;
        return false;
    };

    std::vector<double> data_d{-3.0, -4.0};
    ppp::Column col_d{data_d, "Key"};

    if (col_d.LNorm(2) != 5.0) {
        FailNotification(col_d, "TestNorm");
        (*fails)++;
        std::cout << "L-2 Failed" << col_d.LNorm(2) << std::endl;
        return false;
    };

    std::vector<std::complex<double>> data_c{-3.0, -4.0};
    ppp::Column col_c{data_c, "Key"};

    if (col_c.LNorm(std::nullopt) != 4.0) {
        FailNotification(col_d, "TestNorm");
        (*fails)++;
        std::cout << "L-inf Failed" << col_d.LNorm(std::nullopt) << std::endl;
        return false;
    };

    if (col_c.LNorm(0) != 2.0) {
        FailNotification(col_d, "TestNorm");
        (*fails)++;
        std::cout << "L-0 Failed" << col_d.LNorm(0) << std::endl;
        return false;
    };

    PassNotification(col_c, "TestNorm");
    (*passes)++;
    return true;
}

}  // namespace

bool ColumnMasterTest(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
    return TestConstruction(passes, fails) && TestAddition(passes, fails) &&
           TestIndexing(passes, fails) && TestSum(passes, fails) &&
           TestComparison(passes, fails) && TestSubtraction(passes, fails) &&
           TestDot(passes, fails) && TestAppend(passes, fails) &&
           TestScale(passes, fails) && TestNorm(passes, fails);
}

}  // namespace column_test
