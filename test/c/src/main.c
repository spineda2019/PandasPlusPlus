#include "ppp_column.h"

int main(int argc, char** argv) {
    float nums[3] = {3.0f, 2.0f, 6.7f};
    const char key[] = "Column";

    FColumnHandle col = NewFColumn(nums, 3, key);
    FColumnHandle col2 = NewFColumn(nums, 3, key);

    PrintFColumn(col);

    FColumnHandle col3 = AddFColumns(col, col2);

    PrintFColumn(col3);

    int ints[3] = {3, 2, 7};
    NewColumn_int(ints, 3, key);

    return 0;
}
