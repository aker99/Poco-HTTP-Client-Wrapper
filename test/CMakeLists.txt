# Creating Sets of files for formating path and extension 

## Utility Files
set(UTILITY_TEST_DIR)
list(TRANSFORM UTILITY_TEST_DIR PREPEND Utils/)

## Default Source
set(D_TEST_FILE HTTPRequestWrapper)

## Formatting path of all sources
set(TEST_SOURCES ${D_TEST_FILE} ${UTILITY_TEST_DIR})
list(TRANSFORM TEST_SOURCES APPEND .test.cpp)
list(TRANSFORM TEST_SOURCES PREPEND source/)

add_executable(${BINARY_T} main.test.cpp ${TEST_SOURCES})

# add_test(NAME ${PROJECT_NAME}-Default-Tests COMMAND ${BINARY_T})

find_package(GTest REQUIRED)
target_link_libraries(${BINARY_T} ${BINARY_LIB} GTest::gtest GTest::gtest_main)
