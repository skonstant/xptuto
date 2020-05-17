cmake_minimum_required(VERSION 3.10)
project(xpcommon)

include_directories(gen)

file(GLOB_RECURSE SRC "*.cpp")
file(GLOB_RECURSE HDR "*.h")
file(GLOB_RECURSE HDR_GEN "*.hpp")
if (APPLE)
    include_directories(objc)
    add_library(xpcommon ${SRC} ${HDR} ${HDR_GEN} impl/apple_http_client.mm impl/apple_threads.mm)
    set_property(TARGET xpcommon APPEND_STRING PROPERTY
            COMPILE_FLAGS "-fobjc-arc")
else (APPLE)
    add_library(xpcommon ${SRC} ${HDR} ${HDR_GEN})
endif (APPLE)

