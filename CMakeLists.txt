cmake_minimum_required(VERSION 3.6)
project(deobfuscator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/main.cpp src/Decrypter.cpp src/Helpers.cpp src/Deobfuscator.cpp src/TLConstructors.h
    )


link_libraries(crypto pthread z)

add_executable(deobfuscator ${SOURCE_FILES})
