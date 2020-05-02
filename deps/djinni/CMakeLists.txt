project(djinni)

include_directories(./support-lib/jni)

set(SOURCE_FILES ./support-lib/jni/djinni_support.cpp ./support-lib/jni/djinni_main.cpp)

set(HEADER_FILES
        ./support-lib/jni/*.h
        )

add_library(djinni ${SOURCE_FILES})