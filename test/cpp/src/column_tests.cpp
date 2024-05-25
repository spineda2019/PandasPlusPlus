#include "include/column_tests.hpp"

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

}  // namespace

bool ColumnMasterTest(const std::unique_ptr<std::size_t>& passes,
                      const std::unique_ptr<std::size_t>& fails) {
    return TestConstruction(passes, fails) && TestAddition(passes, fails) &&
           TestIndexing(passes, fails) && TestSum(passes, fails) &&
           TestComparison(passes, fails) && TestSubtraction(passes, fails) &&
           TestDot(passes, fails);
}

}  // namespace column_test
