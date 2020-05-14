// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from main.djinni

#include "native_http_client.hpp"  // my header
#include "Marshal.hpp"
#include "native_http_callback.hpp"

namespace djinni_generated {

NativeHttpClient::NativeHttpClient() : ::djinni::JniInterface<::xptuto::HttpClient, NativeHttpClient>("org/example/xptuto/HttpClient$CppProxy") {}

NativeHttpClient::~NativeHttpClient() = default;

NativeHttpClient::JavaProxy::JavaProxy(JniType j) : Handle(::djinni::jniGetThreadEnv(), j) { }

NativeHttpClient::JavaProxy::~JavaProxy() = default;

void NativeHttpClient::JavaProxy::get(const std::string & c_url, const std::shared_ptr<::xptuto::HttpCallback> & c_callback) {
    auto jniEnv = ::djinni::jniGetThreadEnv();
    ::djinni::JniLocalScope jscope(jniEnv, 10);
    const auto& data = ::djinni::JniClass<::djinni_generated::NativeHttpClient>::get();
    jniEnv->CallVoidMethod(Handle::get().get(), data.method_get,
                           ::djinni::get(::djinni::String::fromCpp(jniEnv, c_url)),
                           ::djinni::get(::djinni_generated::NativeHttpCallback::fromCpp(jniEnv, c_callback)));
    ::djinni::jniExceptionCheck(jniEnv);
}

CJNIEXPORT void JNICALL Java_org_example_xptuto_HttpClient_00024CppProxy_nativeDestroy(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        delete reinterpret_cast<::djinni::CppProxyHandle<::xptuto::HttpClient>*>(nativeRef);
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

CJNIEXPORT void JNICALL Java_org_example_xptuto_HttpClient_00024CppProxy_native_1get(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef, jstring j_url, jobject j_callback)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::xptuto::HttpClient>(nativeRef);
        ref->get(::djinni::String::toCpp(jniEnv, j_url),
                 ::djinni_generated::NativeHttpCallback::toCpp(jniEnv, j_callback));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

}  // namespace djinni_generated
