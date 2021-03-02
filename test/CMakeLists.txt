set(BINARY ${CMAKE_PROJECT_NAME}_test)


# Set Utility Test
set(UTILITY_TEST_DIR URLParser.test.cpp)
list(TRANSFORM UTILITY_TEST_DIR PREPEND Utils/)

# Default Source
set(D_TEST_FILE HTTPRequestWrapper.test.cpp)

# Formatting path of all sources
set(TEST_SOURCES ${D_SOURCE_FILE} ${UTILITY_SOURCE_DIR})
list(TRANSFORM TEST_SOURCES PREPEND source/)


add_executable(${BINARY} main.cpp ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

# include(GoogleTest)
find_package(GTest REQUIRED)
target_link_libraries(${BINARY} ${CMAKE_PROJECT_NAME}_lib GTest::gtest GTest::gtest_main)


find_package(OpenSSL REQUIRED SSL Crypto)
target_link_libraries(${BINARY_R} OpenSSL::SSL OpenSSL::Crypto)

find_package(Poco CONFIG REQUIRED Net Util NetSSL)
target_link_libraries(${BINARY_R} Poco::Foundation Poco::Util Poco::Net Poco::NetSSL)