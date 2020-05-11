#include <jni.h>
#include <string>
#include "djinni_support.hpp"
#include <signal.h>
#include <unistd.h>

// Called when library is loaded by the first class which uses it.
CJNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * jvm, void * /*reserved*/) {
    djinni::jniInit(jvm);
    return JNI_VERSION_1_6;
}

// (Potentially) called when library is about to be unloaded.
CJNIEXPORT void JNICALL JNI_OnUnload(JavaVM * /*jvm*/, void * /*reserved*/) {
    djinni::jniShutdown();
}

