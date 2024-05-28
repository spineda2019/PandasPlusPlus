#include <stdio.h>

#include "common_defs.h"
#include "ppp_column.h"

int TestConstruction(int* passes, int* fails) {
    float nums[3] = {3.0f, 2.0f, 6.7f};
    const char key[] = "Column";

    FColumn col = NewFColumn(nums, 3, key);

    if (col) {
        DeleteFColumn(col);
        SUCCESS_PRINT("TestConstruction");
        (*passes)++;
        return 1;
    } else {
        FAILURE_PRINT("TestConstruction");
        (*fails)++;
        return 0;
    }
}

int TestPrint(int* passes, int* fails) {
    float nums[3] = {3.0f, 2.0f, 6.7f};
    const char key[] = "Column";

    FColumn col = NewFColumn(nums, 3, key);
    printf("Visually confirm column is {3.0, 2.0, 6.7}\n");
    PrintFColumn(col);

    DeleteFColumn(col);
    SUCCESS_PRINT("TestPrint");

    (*passes)++;

    return 1;
}

int TestAddition(int* passes, int* fails) {
    float nums[3] = {3.0f, 2.0f, 6.7f};
    const char key[] = "Column";

    FColumn col = NewFColumn(nums, 3, key);
    FColumn col2 = NewFColumn(nums, 3, key);

    FColumn col3 = AddFColumns(col, col2);

    if (!col3) {
        DeleteFColumn(col);
        DeleteFColumn(col2);
        FAILURE_PRINT("TestAddition");
        (*fails)++;
        return 0;
    } else {
        printf("Visually confirm column is {6.0, 4.0, 13.4}\n");
        PrintFColumn(col3);

        DeleteFColumn(col);
        DeleteFColumn(col2);
        DeleteFColumn(col3);

        SUCCESS_PRINT("TestAddition");
        (*passes)++;
        return 1;
    }
}

int TestSum(int* passes, int* fails) {
    float nums[3] = {3.0f, 2.0f, 6.7f};
    const char key[] = "Column";

    FColumn col = NewFColumn(nums, 3, key);

    if (!col) {
        FAILURE_PRINT("TestSum");
        (*fails)++;
        return 0;
    } else {
        float sum = SumFColumn(col);

        if (sum != (nums[0] + nums[1] + nums[2])) {
            DeleteFColumn(col);
            FAILURE_PRINT("TestSum");
            (*fails)++;
            return 0;
        } else {
            DeleteFColumn(col);
            SUCCESS_PRINT("TestSum");
            printf("Sum: %f\n\n", sum);
            (*passes)++;
            return 1;
        }
    }
}

int TestSize(int* passes, int* fails) {
    const float nums[3] = {3.0f, 2.0f, 6.7f};
    const char key[] = "Column";

    FColumn col = NewFColumn(nums, 3, key);

    if (!col) {
        FAILURE_PRINT("TestSize");
        (*fails)++;
        return 0;
    } else {
        size_t size = FColumnSize(col);

        if (size != 3) {
            FAILURE_PRINT("TestSize");
            (*fails)++;
            return 0;
        } else {
            SUCCESS_PRINT("TestSize");
            (*passes)++;
            return 0;
        }
    }
}
