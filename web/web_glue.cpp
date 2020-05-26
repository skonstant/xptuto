// when we have stuff to marshall
#include <emscripten.h>
#include <emscripten/bind.h>
#include <optional>
#include <user.hpp>
#include <repo.hpp>
#include <xptuto.hpp>
#include <get_user_cb.hpp>
#include <get_repos_cb.hpp>
#include <filesystem>

#include "web_http_client.hpp"
#include "web_threads.hpp"

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
            (double) (std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch())).count());
}

std::shared_ptr<xptuto::Xptuto> createInstance() {
    const static auto dirname = "/cache";

    if (!std::filesystem::exists(dirname)) {
        std::filesystem::create_directory(dirname);
    }
    return xptuto::Xptuto::make_instance(std::make_shared<WebHttpClient>(), std::make_shared<WebThreads>(), dirname);
}

class JSInterface {

public:
    explicit JSInterface(emscripten::val callbacks) : myCallbacks(callbacks) {}

    emscripten::val myCallbacks;
};

class JSGetUserCb : public xptuto::GetUserCb, JSInterface {
public:
    explicit JSGetUserCb(emscripten::val callbacks) : JSInterface(std::move(callbacks)) {}

    void on_success(const User &user) override {
        emscripten::val on_successCB = myCallbacks["on_success"];
        on_successCB(user);
    }

    void on_error(const std::string &error) override {
        emscripten::val on_errorCB = myCallbacks["on_error"];
        on_errorCB(error);
    }
};

class JSGetReposCb : public xptuto::GetReposCb, JSInterface {
public:
    explicit JSGetReposCb(emscripten::val callbacks) : JSInterface(std::move(callbacks)) {}

    void on_success(const std::vector<Repo> &repos, const User &user) override {
        emscripten::val on_successCB = myCallbacks["on_success"];
        on_successCB(repos, user);
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
            .function("millis", &toJsDate);

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
            .property("descr", &Repo::descr)
            .property("created_at", &Repo::created_at);

    emscripten::class_<Xptuto>("Xptuto")
            .smart_ptr<std::shared_ptr<Xptuto>>("Xptuto")
            .function("get_user", &Xptuto::get_user)
            .function("get_repos_for_user", &Xptuto::get_repos_for_user)
            .function("get_repos_for_user_name", &Xptuto::get_repos_for_user_name);

    emscripten::class_<GetUserCb>("GetUserCb")
            .smart_ptr<std::shared_ptr<GetUserCb>>("GetUserCb");

    emscripten::class_<JSGetUserCb, emscripten::base<GetUserCb>>("JSGetUserCb")
            .smart_ptr_constructor("JSGetUserCb", &std::make_shared<JSGetUserCb, emscripten::val>);

    emscripten::class_<GetReposCb>("GetReposCb")
            .smart_ptr<std::shared_ptr<GetReposCb>>("GetReposCb");

    emscripten::class_<JSGetReposCb, emscripten::base<GetReposCb>>("JSGetReposCb")
            .smart_ptr_constructor("JSGetReposCb", &std::make_shared<JSGetReposCb, emscripten::val>);

}