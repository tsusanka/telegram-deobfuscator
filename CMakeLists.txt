cmake_minimum_required(VERSION 3.6)
project(c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp Decrypter.cpp helpers.cpp Deobfuscator.cpp TLConstructors.h
    )


link_libraries(crypto pthread z)

add_executable(c ${SOURCE_FILES})
