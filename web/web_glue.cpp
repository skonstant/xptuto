// when we have stuff to marshall
#include <emscripten.h>
#include <emscripten/bind.h>
#include <optional>
#include <user.hpp>
#include <repo.hpp>
#include <xptuto.hpp>
#include <get_users_cb.hpp>

#include "web_http_client.hpp"

using namespace xptuto;

template<typename OptionalType>
struct OptionalAccess {
    static emscripten::val value(
            const OptionalType &v) {
        return emscripten::val(v.value());
    }

    static bool has_value(OptionalType &v) {
        return v.has_value();
    }
};

template<typename V>
emscripten::class_<std::optional<V>> register_optional(const char *name) {
    typedef std::optional<V> OptionalType;

    return emscripten::class_<std::optional<V>>(name)
            .function("value", &OptionalAccess<OptionalType>::value)
            .function("has_value", &OptionalAccess<OptionalType>::has_value);
}

emscripten::val toJsDate(std::chrono::system_clock::time_point timePoint) {
    return emscripten::val(
            (long) (std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch())).count());
}

std::shared_ptr<xptuto::Xptuto> createInstance() {
    return xptuto::Xptuto::make_instance(std::make_shared<WebHttpClient>());
}

class JSInterface {

public:
    explicit JSInterface(emscripten::val callbacks) : myCallbacks(callbacks) {}

    emscripten::val myCallbacks;
};

class JSGetUsersCb : public xptuto::GetUsersCb, JSInterface {
public:
    explicit JSGetUsersCb(emscripten::val callbacks) : JSInterface(std::move(callbacks)) {}

    void on_success(const std::vector<User> &users) override {
        emscripten::val on_successCB = myCallbacks["on_success"];
        on_successCB(users);
    }

    void on_error(const std::string &error) override {
        emscripten::val on_errorCB = myCallbacks["on_error"];
        on_errorCB(error);
    }
};

EMSCRIPTEN_BINDINGS(xptuto) {
    emscripten::register_vector<User>("VectorUser");
    emscripten::register_vector<Repo>("VectorRepo");
    register_optional<std::string>("OptionalString");

    emscripten::function("createInstance", &createInstance);

    emscripten::class_<std::chrono::system_clock::time_point>("TimePoint")
            .function("sec", &toJsDate);

    emscripten::class_<User>("User")
            .property("login", &User::login)
            .property("id", &User::id)
            .property("avatar_url", &User::avatar_url)
            .property("created_at", &User::created_at);

    emscripten::class_<Repo>("Repo")
            .property("id", &Repo::id)
            .property("name", &Repo::name)
            .property("full_name", &Repo::full_name)
            .property("owner", &Repo::owner)
            .property("priv", &Repo::priv)
            .property("html_url", &Repo::html_url)
            .property("descr", &Repo::descr)
            .property("created_at", &Repo::created_at)
            .property("updated_at", &Repo::updated_at)
            .property("pushed_at", &Repo::pushed_at);

    emscripten::class_<Xptuto>("Xptuto")
        .smart_ptr<std::shared_ptr<Xptuto>>("Xptuto")
        .function("get_users", &Xptuto::get_users)
        .function("get_repos_for_user", &Xptuto::get_repos_for_user);

    emscripten::class_<GetUsersCb>("GetUsersCb")
            .smart_ptr<std::shared_ptr<GetUsersCb>>("GetUsersCb");

    emscripten::class_<JSGetUsersCb, emscripten::base<GetUsersCb>>("JSGetUsersCb")
            .smart_ptr_constructor("JSGetUsersCb", &std::make_shared<JSGetUsersCb, emscripten::val>);

}