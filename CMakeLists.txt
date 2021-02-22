cmake_minimum_required(VERSION 3.0.0)
project(poco-http-curl VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Set Utility Source
set(UTILITY_SOURCE_DIR Logger.cpp Thread.cpp URLParser.cpp)
list(TRANSFORM UTILITY_SOURCE_DIR PREPEND Utils/)

# Default Source
set(D_SOURCE_FILE PocoCurlApp.cpp CustomHttpSessionFactory.cpp HTTPRequestPool.cpp HTTPRequestPoolRunnable.cpp HTTPSession.cpp)

# Formatting path of all sources
set(SOURCE_FILE ${D_SOURCE_FILE} ${UTILITY_SOURCE_DIR})
list(TRANSFORM SOURCE_FILE PREPEND source/)

add_executable(poco-http-curl main.cpp ${SOURCE_FILE})


target_include_directories(poco-http-curl PUBLIC ${CMAKE_SOURCE_DIR}/include)

find_package(OpenSSL REQUIRED)
target_link_libraries(poco-http-curl PRIVATE OpenSSL::SSL OpenSSL::Crypto)

find_package(Poco CONFIG REQUIRED Net Util NetSSL)
target_link_libraries(poco-http-curl PRIVATE Poco::Foundation Poco::Util Poco::Net Poco::NetSSL)