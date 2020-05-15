// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from main.djinni

#pragma once

#include "djinni_support.hpp"
#include "http_response.hpp"

namespace djinni_generated {

class NativeHttpResponse final {
public:
    using CppType = ::xptuto::HttpResponse;
    using JniType = jobject;

    using Boxed = NativeHttpResponse;

    ~NativeHttpResponse();

    static CppType toCpp(JNIEnv* jniEnv, JniType j);
    static ::djinni::LocalRef<JniType> fromCpp(JNIEnv* jniEnv, const CppType& c);

private:
    NativeHttpResponse();
    friend ::djinni::JniClass<NativeHttpResponse>;

    const ::djinni::GlobalRef<jclass> clazz { ::djinni::jniFindClass("org/example/xptuto/HttpResponse") };
    const jmethodID jconstructor { ::djinni::jniGetMethodID(clazz.get(), "<init>", "(Ljava/lang/String;I)V") };
    const jfieldID field_body { ::djinni::jniGetFieldID(clazz.get(), "body", "Ljava/lang/String;") };
    const jfieldID field_code { ::djinni::jniGetFieldID(clazz.get(), "code", "I") };
};

}  // namespace djinni_generated
