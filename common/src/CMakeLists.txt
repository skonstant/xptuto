cmake_minimum_required(VERSION 3.10)
project(xpcommon)

include_directories(gen)

file(GLOB_RECURSE SRC "*.cpp")
file(GLOB_RECURSE HDR "*.h")
file(GLOB_RECURSE HDR_GEN "*.hpp")

find_package (SQLite3)
if (SQLITE3_FOUND)
    include_directories(${SQLITE3_INCLUDE_DIRS})
else (SQLITE3_FOUND)
    # add amalgamation code and include
    set(SRC ${SRC} sqlite-amalgamation-3310100/sqlite3.c)
    include_directories(sqlite-amalgamation-3310100)
endif (SQLITE3_FOUND)

if (APPLE)
    include_directories(objc)
    add_library(xpcommon ${SRC} ${HDR} ${HDR_GEN} impl/apple_http_client.mm impl/apple_threads.mm)
    set_property(TARGET xpcommon APPEND_STRING PROPERTY
            COMPILE_FLAGS "-fobjc-arc")
else (APPLE)
    add_library(xpcommon ${SRC} ${HDR} ${HDR_GEN})
endif (APPLE)

if (EMSCRIPTEN)
    set_target_properties(xpcommon PROPERTIES COMPILE_FLAGS " -s USE_PTHREADS=1 -s DISABLE_EXCEPTION_CATCHING=0")
endif(EMSCRIPTEN)

