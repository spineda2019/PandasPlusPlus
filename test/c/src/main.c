#include <stdio.h>

#include "column_test.h"

int main(int argc, char** argv) {
    int passes = 0;
    int fails = 0;

    TestGeneric(&passes, &fails) && TestConstruction(&passes, &fails) &&
        TestPrint(&passes, &fails) && TestAddition(&passes, &fails) &&
        TestSum(&passes, &fails) && TestSize(&passes, &fails);

    printf("Total Passes: %i\n", passes);
    printf("Total Fails: %i\n", fails);

    if (fails == 0) {
        printf("All Tests Passed!!\n");
        return 0;
    } else {
        printf("Some tests failed...\n");
        return -1;
    }
}
