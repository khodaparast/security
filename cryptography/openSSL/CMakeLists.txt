cmake_minimum_required(VERSION 3.10)

# 1. set the project name
project(FKHEncryption VERSION 1.0) 

set(CMAKE_CXX_FLAGS "-lcrypto -lpthread -lssl  -lz -ldl")

# 2. set the project sources and add executable
file(GLOB SOURCES src/*.cc)
set(SOURCE_FILES ${SOURCES})
add_executable(FKHEncryption ${SOURCE_FILES})


include_directories(lib)

set(OPENSSL_ROOT_DIR  /bin)
find_package(OpenSSL REQUIRED)




target_link_libraries(FKHEncryption OpenSSL::SSL)

