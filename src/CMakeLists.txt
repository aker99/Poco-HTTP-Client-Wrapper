set(BINARY ${CMAKE_PROJECT_NAME})
set(BINARY_R ${BINARY}_run)

# Set Utility Source
set(UTILITY_SOURCE_DIR Logger.cpp Thread.cpp URLParser.cpp)
list(TRANSFORM UTILITY_SOURCE_DIR PREPEND Utils/)

# Default Source
set(D_SOURCE_FILE PocoCurlApp.cpp CustomHttpSessionFactory.cpp HTTPRequestPool.cpp HTTPRequestPoolRunnable.cpp HTTPRequestWrapper.cpp)

# Formatting path of all sources
set(SOURCE_FILE ${D_SOURCE_FILE} ${UTILITY_SOURCE_DIR})
list(TRANSFORM SOURCE_FILE PREPEND source/)

add_executable(${BINARY_R} main.cpp ${SOURCE_FILE})

target_include_directories(${BINARY_R} PUBLIC include)

# add_library(${BINARY}_lib STATIC ${SOURCE_FILE})

find_package(OpenSSL REQUIRED SSL Crypto)
target_link_libraries(${BINARY_R} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

find_package(Poco CONFIG REQUIRED Net Util NetSSL)
target_link_libraries(${BINARY_R} PRIVATE Poco::Foundation Poco::Util Poco::Net Poco::NetSSL)

