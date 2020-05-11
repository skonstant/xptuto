project(xptuto-web)

set(SOURCE_FILES web_glue.cpp)

link_libraries(xpcommon)

include_directories(../common/src/gen)

add_executable(xptuto-web ${SOURCE_FILES})

set_target_properties(xptuto-web PROPERTIES LINK_FLAGS "-o common-web.js --bind")
