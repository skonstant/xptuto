// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from main.djinni

#include "native_repo.hpp"  // my header
#include "Marshal.hpp"

namespace djinni_generated {

NativeRepo::NativeRepo() = default;

NativeRepo::~NativeRepo() = default;

auto NativeRepo::fromCpp(JNIEnv* jniEnv, const CppType& c) -> ::djinni::LocalRef<JniType> {
    const auto& data = ::djinni::JniClass<NativeRepo>::get();
    auto r = ::djinni::LocalRef<JniType>{jniEnv->NewObject(data.clazz.get(), data.jconstructor,
                                                           ::djinni::get(::djinni::I32::fromCpp(jniEnv, c.id)),
                                                           ::djinni::get(::djinni::String::fromCpp(jniEnv, c.name)),
                                                           ::djinni::get(::djinni::String::fromCpp(jniEnv, c.full_name)),
                                                           ::djinni::get(::djinni::I32::fromCpp(jniEnv, c.owner)),
                                                           ::djinni::get(::djinni::Bool::fromCpp(jniEnv, c.priv)),
                                                           ::djinni::get(::djinni::Optional<std::optional, ::djinni::String>::fromCpp(jniEnv, c.descr)),
                                                           ::djinni::get(::djinni::Date::fromCpp(jniEnv, c.created_at)))};
    ::djinni::jniExceptionCheck(jniEnv);
    return r;
}

auto NativeRepo::toCpp(JNIEnv* jniEnv, JniType j) -> CppType {
    ::djinni::JniLocalScope jscope(jniEnv, 8);
    assert(j != nullptr);
    const auto& data = ::djinni::JniClass<NativeRepo>::get();
    return {::djinni::I32::toCpp(jniEnv, jniEnv->GetIntField(j, data.field_id)),
            ::djinni::String::toCpp(jniEnv, (jstring)jniEnv->GetObjectField(j, data.field_name)),
            ::djinni::String::toCpp(jniEnv, (jstring)jniEnv->GetObjectField(j, data.field_fullName)),
            ::djinni::I32::toCpp(jniEnv, jniEnv->GetIntField(j, data.field_owner)),
            ::djinni::Bool::toCpp(jniEnv, jniEnv->GetBooleanField(j, data.field_priv)),
            ::djinni::Optional<std::optional, ::djinni::String>::toCpp(jniEnv, (jstring)jniEnv->GetObjectField(j, data.field_descr)),
            ::djinni::Date::toCpp(jniEnv, jniEnv->GetObjectField(j, data.field_createdAt))};
}

}  // namespace djinni_generated