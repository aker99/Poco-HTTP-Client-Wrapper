cmake_minimum_required(VERSION 3.0.0)
project(poco-http-curl VERSION 0.1.0)

# include(GoogleTest)
# enable_testing()

set(CMAKE_CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(BINARY_R  ${PROJECT_NAME})
set(BINARY_T ${PROJECT_NAME}-test)
set(BINARY_LIB ${PROJECT_NAME}-lib)
# include_directories(src)


# Cretaing List of components used of OpenSSL 
set(OPENSSL_LIB SSL Crypto)
set(OPENSSL_LIB_LINK ${OPENSSL_LIB})
list(TRANSFORM OPENSSL_LIB_LINK PREPEND OpenSSL::)

# Cretaing List of components used of Poco 
set(POCO_LIB Foundation Net Util NetSSL)
set(POCO_LIB_LINK ${POCO_LIB})
list(TRANSFORM POCO_LIB_LINK PREPEND Poco::)


# Adding Sub Directories to be parsed and look for CMakeList.txt in those dir
add_subdirectory(src)
add_subdirectory(test)