cmake_minimum_required(VERSION 3.4.1)
project(xptuto)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif (CCACHE_FOUND)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
    include_directories("${EMSCRIPTEN_PREFIX}/system/include")
    add_subdirectory(web)
endif(EMSCRIPTEN)

add_subdirectory(common/src)
add_subdirectory(test)
