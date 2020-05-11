// when we have stuff to marshall
#include <emscripten.h>
#include <emscripten/bind.h>

#include <user.hpp>
#include <repo.hpp>
#include <xptuto.hpp>

using namespace xptuto;

EMSCRIPTEN_BINDINGS(xptuto) {
    emscripten::register_vector<User>("VectorUser");
    emscripten::register_vector<Repo>("VectorRepo");

    emscripten::class_<User>("User")
            .property("login", &User::login)
            .property("id", &User::id)
            .property("avatar_url", &User::avatar_url)
            .property("gravatar_id", &User::gravatar_id)
            .property("url", &User::url)
            .property("html_url", &User::html_url)
            .property("type", &User::type)
            .property("site_admin", &User::site_admin)
            .property("name", &User::name)
            .property("company", &User::company)
            .property("bio", &User::bio)
            .property("public_repos", &User::public_repos)
            .property("public_gists", &User::public_gists)
            .property("followers", &User::followers)
            .property("following", &User::following)
            .property("created_at", &User::created_at)
            .property("updated_at", &User::updated_at);

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
        .class_function("make_instance", &Xptuto::make_instance)
        .function("get_users", &Xptuto::get_users)
        .function("get_repos_for_user", &Xptuto::get_repos_for_user);
}