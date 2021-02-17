cmake_minimum_required(VERSION 3.0.0)
project(poco-http-curl VERSION 0.1.0)

include(CTest)
enable_testing()


add_executable(poco-http-curl main.cpp lib/CustomHttpClient.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(poco-http-curl PUBLIC ${CMAKE_SOURCE_DIR}/headers)

find_package(Poco CONFIG REQUIRED Net Util)
target_link_libraries(poco-http-curl PRIVATE Poco::Net Poco::Util)