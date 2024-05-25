project(pytest)

add_test(NAME PyTest COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/test.py")
