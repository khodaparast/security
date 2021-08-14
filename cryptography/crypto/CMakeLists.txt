
project(cryptographyFKH)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(OpenSSL REQUIRED)


add_executable(AESENC Crypto.cpp)

target_link_libraries(AESENC   OpenSSL::Crypto OpenSSL::SSL )
