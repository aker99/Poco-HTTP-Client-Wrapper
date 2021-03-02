set(BINARY ${CMAKE_PROJECT_NAME})
set(BINARY_R ${BINARY}_run)

# Set Utility Files
set(UTILITY_FILES Logger Thread URLParser)
list(TRANSFORM UTILITY_FILES PREPEND Utils/)

# Default Files
set(D_FILE PocoCurlApp CustomHttpSessionFactory HTTPRequestPool HTTPRequestPoolRunnable HTTPRequestWrapper)

# Merging All Files
set(ALL_FILE ${D_FILE} ${UTILITY_FILES})

set(SOURCE_FILE ${ALL_FILE})
set(HEADER_FILE ${ALL_FILE})

# Formatting path of all sources
list(TRANSFORM SOURCE_FILE PREPEND source/)
list(TRANSFORM SOURCE_FILE APPEND .cpp)

# Formatting path of all include
list(TRANSFORM HEADER_FILE PREPEND include/)
list(TRANSFORM HEADER_FILE APPEND .h)

add_executable(${BINARY_R} main.cpp ${SOURCE_FILE})

target_include_directories(${BINARY_R} PUBLIC include)

add_library(${BINARY}_lib STATIC ${HEADER_FILE} ${SOURCE_FILE})
# target_include_directories(${BINARY}_lib STATIC include)

find_package(OpenSSL REQUIRED SSL Crypto)
target_link_libraries(${BINARY_R} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

find_package(Poco CONFIG REQUIRED Net Util NetSSL)
target_link_libraries(${BINARY_R} PRIVATE Poco::Foundation Poco::Util Poco::Net Poco::NetSSL)

