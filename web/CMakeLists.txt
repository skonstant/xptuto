project(xptuto-web)

set(SOURCE_FILES web_glue.cpp)

link_libraries(xpcommon)

include_directories(../common/src/gen ../common/src/impl)

add_executable(xptuto-web ${SOURCE_FILES})
set_target_properties(xptuto-web PROPERTIES COMPILE_FLAGS " -s USE_PTHREADS=1 -s ALLOW_BLOCKING_ON_MAIN_THREAD=1 -s PTHREAD_POOL_SIZE=2 ")
set_target_properties(xptuto-web PROPERTIES LINK_FLAGS "-o common-web.js --bind -s DISABLE_EXCEPTION_CATCHING=0 -s FETCH=1 -s USE_PTHREADS=1 -s ALLOW_BLOCKING_ON_MAIN_THREAD=1 -s PTHREAD_POOL_SIZE=2 ")
