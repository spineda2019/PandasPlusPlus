#include "CInterface.h"

int main(int argc, char** argv) {
    float nums[3] = {3.0f, 2.0f, 6.7f};
    const char key[] = "Column";

    FColumnHandle col = NewFColumn(nums, 3, key);

    PrintFColumn(col);

    return 0;
}
