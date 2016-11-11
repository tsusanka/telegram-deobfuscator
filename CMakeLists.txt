cmake_minimum_required(VERSION 3.6)
project(c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        unobfuscate.cpp decrypt.cpp helpers.cpp
        tgnet)

#include_directories(/usr/include/openssl/)
link_libraries(crypto)

add_executable(c ${SOURCE_FILES})
