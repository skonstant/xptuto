cmake_minimum_required(VERSION 3.3)
project(djinni)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

include_directories(./support-lib/jni)

set(SOURCE_FILES ./support-lib/jni/djinni_support.cpp ./support-lib/jni/djinni_main.cpp)

set(HEADER_FILES
        ./support-lib/jni/*.h
        )

add_library(djinni ${SOURCE_FILES})