# Creating Sets of files for formating path and extension 

## Set Utility Files
set(UTILITY_FILES Logger Thread URLParser)
list(TRANSFORM UTILITY_FILES PREPEND Utils/)

## Default Files
set(D_FILE PocoCurlApp CustomHttpSessionFactory HTTPRequestPool HTTPRequestPoolRunnable HTTPRequestWrapper)

## Merging All Files
set(ALL_FILE ${D_FILE} ${UTILITY_FILES})

set(SOURCE_FILE ${ALL_FILE})
set(HEADER_FILE ${ALL_FILE})

## Formatting path of all sources
list(TRANSFORM SOURCE_FILE PREPEND source/)
list(TRANSFORM SOURCE_FILE APPEND .cpp)

## Formatting path of all include
# list(TRANSFORM HEADER_FILE PREPEND include/)
# list(TRANSFORM HEADER_FILE APPEND .h)

# Creating Library so that the binaries could be reused for testing module

add_library(${BINARY_LIB} STATIC ${SOURCE_FILE})

## Finding and linking 3rd party libaries
find_package(OpenSSL REQUIRED ${OPENSSL_LIB})
find_package(Poco CONFIG REQUIRED ${POCO_LIB})
target_link_libraries(${BINARY_LIB} PRIVATE ${OPENSSL_LIB_LINK} ${POCO_LIB_LINK})

## Adding path to find header files
target_include_directories(${BINARY_LIB} PUBLIC include)

# Creating an excuatble file 

add_executable(${BINARY_R} main.cpp)

## Linking Poco::Util and Custom Library (Created Above ${BINARY_LIB})
target_link_libraries(${BINARY_R} PRIVATE ${BINARY_LIB} Poco::Util)


