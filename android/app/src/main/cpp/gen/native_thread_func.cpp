// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from main.djinni

#include "native_thread_func.hpp"  // my header

namespace djinni_generated {

NativeThreadFunc::NativeThreadFunc() : ::djinni::JniInterface<::xptuto::ThreadFunc, NativeThreadFunc>("org/example/xptuto/ThreadFunc$CppProxy") {}

NativeThreadFunc::~NativeThreadFunc() = default;


CJNIEXPORT void JNICALL Java_org_example_xptuto_ThreadFunc_00024CppProxy_nativeDestroy(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        delete reinterpret_cast<::djinni::CppProxyHandle<::xptuto::ThreadFunc>*>(nativeRef);
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

CJNIEXPORT void JNICALL Java_org_example_xptuto_ThreadFunc_00024CppProxy_native_1run(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::xptuto::ThreadFunc>(nativeRef);
        ref->run();
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

}  // namespace djinni_generated