cmake_minimum_required(VERSION 3.4.1)
project(xptuto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
    include_directories("${EMSCRIPTEN_PREFIX}/system/include")
    add_subdirectory(web)
endif(EMSCRIPTEN)

add_subdirectory(common/src)
