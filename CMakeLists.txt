cmake_minimum_required(VERSION 3.6)
project(deobfuscator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp Decrypter.cpp Helpers.cpp Deobfuscator.cpp TLConstructors.h
    )


link_libraries(crypto pthread z)

add_executable(deobfuscator ${SOURCE_FILES})
