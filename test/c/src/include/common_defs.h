#define FAILURE_PRINT(function_name)                                           \
    printf("Test %s failed...\n", function_name);                              \
    printf(                                                                    \
        "____________________________________________________________________" \
        "____________\n\n");

#define SUCCESS_PRINT(function_name)                                           \
    printf("Test %s passed!\n", function_name);                                \
    printf(                                                                    \
        "____________________________________________________________________" \
        "____________\n\n");
