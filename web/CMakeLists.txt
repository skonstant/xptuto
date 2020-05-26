project(xptuto-web)

set(SOURCE_FILES web_glue.cpp)

link_libraries(xpcommon)

include_directories(../common/src/gen ../common/src/impl)

add_executable(xptuto-web ${SOURCE_FILES})

set_target_properties(xptuto-web PROPERTIES COMPILE_FLAGS " -s USE_PTHREADS=1 -s DISABLE_EXCEPTION_CATCHING=0")
set_target_properties(xptuto-web PROPERTIES LINK_FLAGS "--bind -s DISABLE_EXCEPTION_CATCHING=0 -s FETCH=1 -s USE_PTHREADS=1 -lidbfs.js")
