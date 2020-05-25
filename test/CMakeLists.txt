# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif ()

add_compile_definitions(TEST_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(../common/src/gen ../common/src/impl ../common/src/sqlite_orm)

set(SRC xptuto_test.cpp stubs/http_client_stub.cpp stubs/platform_threads_stub.cpp)

if (APPLE)
    set(EXTRA_LIBS "-framework Foundation")
    add_executable(xptutotest ${SRC} apple_test.mm)
elseif (EMSCRIPTEN)
    add_executable(xptutotest ${SRC} web_test.cpp)
else (APPLE)
    add_executable(xptutotest ${SRC})
endif (APPLE)

find_package (SQLite3)
if (SQLITE3_FOUND)
    if (EXTRA_LIBS)
        set(EXTRA_LIBS "${EXTRA_LIBS} ${SQLite3_LIBRARIES}")
    else (EXTRA_LIBS)
        set(EXTRA_LIBS "${SQLite3_LIBRARIES}")
    endif (EXTRA_LIBS)
else(SQLITE3_FOUND)
    include_directories(../common/src/sqlite-amalgamation-3310100)
endif (SQLITE3_FOUND)

if (EMSCRIPTEN)
    set_target_properties(xptutotest PROPERTIES LINK_FLAGS " -s ALLOW_MEMORY_GROWTH=1 -s USE_PTHREADS=1 -s ALLOW_BLOCKING_ON_MAIN_THREAD=1 -s PTHREAD_POOL_SIZE=2 --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/responses -s FETCH=1 -s DISABLE_EXCEPTION_CATCHING=0")
    set_target_properties(xptutotest PROPERTIES COMPILE_FLAGS " -s USE_PTHREADS=1 -s ALLOW_BLOCKING_ON_MAIN_THREAD=1 -s PTHREAD_POOL_SIZE=2 -s DISABLE_EXCEPTION_CATCHING=0")
    set_target_properties(gtest_main PROPERTIES COMPILE_FLAGS " -s USE_PTHREADS=1 -s ALLOW_BLOCKING_ON_MAIN_THREAD=1 -s PTHREAD_POOL_SIZE=2 -s DISABLE_EXCEPTION_CATCHING=0")
    set_target_properties(gtest PROPERTIES COMPILE_FLAGS " -s USE_PTHREADS=1 -s ALLOW_BLOCKING_ON_MAIN_THREAD=1 -s PTHREAD_POOL_SIZE=2 -s DISABLE_EXCEPTION_CATCHING=0")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif (EMSCRIPTEN)

target_link_libraries(xptutotest gtest_main xpcommon ${EXTRA_LIBS})
add_test(NAME xptuto_test COMMAND xptutotest)