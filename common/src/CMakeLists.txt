cmake_minimum_required(VERSION 3.10)
project(xpcommon)

file(GLOB_RECURSE SRC "*.cpp")
file(GLOB_RECURSE HDR "*.h")
file(GLOB_RECURSE HDR_GEN "*.hpp")

add_library(xpcommon ${SRC} ${HDR} ${HDR_GEN})

