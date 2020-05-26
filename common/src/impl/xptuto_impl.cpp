//
// Created by Stephane Konstantaropoulos on 11/05/2020.
//

#include "xptuto_impl.hpp"

#include <utility>

#include <sqlite3.h>

#include "http_client.hpp"
#include "http_callback_impl.hpp"
#include "get_repos_cb.hpp"
#include "get_user_cb.hpp"
#include "json_parser.hpp"
#include "platform_threads.hpp"
#include "thread_func_impl.hpp"
#include "http_response.hpp"

using namespace xptuto;
using namespace std::chrono_literals;

void XptutoImpl::check_sqlite3() {
    printf("sqlite version : %s", sqlite3_libversion());
}

std::shared_ptr<Xptuto> Xptuto::make_instance(const std::shared_ptr<HttpClient> &client,
        const std::shared_ptr<PlatformThreads> & threads, const std::optional<std::string> &cache_path) {
    XptutoImpl::instance = std::make_shared<XptutoImpl>(client, threads, cache_path);
    return XptutoImpl::instance;
}

std::shared_ptr<Xptuto> Xptuto::get_instance() {
    return XptutoImpl::instance;
}

XptutoImpl::XptutoImpl(std::shared_ptr<HttpClient> cl, std::shared_ptr<PlatformThreads> threads, const std::optional<std::string> &cache_path) :
        client(std::move(cl)), threads(std::move(threads)),
        storage(cache_path ? (cache_path.value() + "/cache.sqlite") : ":memory:") {}
                                                                                                           
void XptutoImpl::get_repos_for_user(const User &user, const std::shared_ptr<GetReposCb> &cb) {
    auto me = shared_from_this();

    client->get("https://api.github.com/users/" + user.login + "/repos",
                std::make_shared<HttpCallbackImpl>([cb, user, me](const std::string_view &body, int32_t code) {
                    if (!std::empty(body)) {
                        std::vector<Repo> repos = nlohmann::json::parse(body);
                        me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb, repos, user](){
                            cb->on_success(repos, user);
                        }));
                    } else {
                        me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb](){
                            cb->on_error("error");
                        }));
                    }
                }, [cb,me](const std::string &) {
                    me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb](){
                        cb->on_error("error");
                    }));
                }));
}

void XptutoImpl::get_user(const std::string &login, const std::shared_ptr<GetUserCb> &cb) {
    auto me = shared_from_this();

    threads->create_thread("get_user", std::make_shared<ThreadFuncImpl>([me, cb, login]() {
        try {
            try {
                auto user = me->storage.get_user(login);
                me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb, user]() {
                    cb->on_success(user);
                }));
                try {
                    auto userOpt = me->get_user_sync(login);
                    if (userOpt) {
                        me->storage.store_user(userOpt.value());
                    }
                } catch (...) {
                    //PASS
                }
            } catch (...) {
                auto user = me->get_user_sync(login);
                if (!user) {
                    me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb]() {
                        cb->on_error("could not load user");
                    }));
                } else {
                    me->storage.store_user(user.value());
                    me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb, user]() {
                        cb->on_success(user.value());
                    }));
                }
            }
        } catch (...) {
            me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb]() {
                cb->on_error("could not load user");
            }));
        }
    }));
}

void XptutoImpl::get_repos_for_user_name(const std::string &username, const std::shared_ptr<GetReposCb> &cb) {
    auto me = shared_from_this();

    threads->create_thread("get_repos_for_user_name", std::make_shared<ThreadFuncImpl>([me, cb, username](){
        try {
            std::optional<User> user;
            try {
                user = me->storage.get_user(username);
                auto repos = me->storage.get_repos(username);
                if (!repos.empty()) {
                    me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb, repos, user]() {
                        cb->on_success(repos, user.value());
                    }));
                    return;
                } else {
                    try {
                        repos = me->get_repos_sync(user.value());
                        me->storage.store_repos(repos);
                        me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb, repos, user]() {
                            cb->on_success(repos, user.value());
                        }));
                    } catch (...) {
                        me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb, user]() {
                            cb->on_error("could not load repos for user: " + user->login);
                        }));
                    }
                }
            } catch (...) {
                user = me->get_user_sync(username);
                if (user) {
                    me->storage.store_user(user.value());
                    try {
                        auto repos = me->get_repos_sync(user.value());
                        me->storage.store_repos(repos);
                        me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb, repos, user]() {
                            cb->on_success(repos, user.value());
                        }));
                    } catch (...) {
                        me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb, user]() {
                            cb->on_error("could not load repos for user: " + user->login);
                        }));
                    }
                } else {
                    me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb]() {
                        cb->on_error("could not load user");
                    }));
                }
            }
        } catch (...) {
            me->threads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([cb](){
                cb->on_error("could not load user");
            }));
        }
    }));
}

std::optional<User> XptutoImpl::get_user_sync(const std::string &login) {
    auto response = client->get_sync("https://api.github.com/users/" + login);
    if (response.code == 200 && response.body && !std::empty(response.body.value())) {
        return nlohmann::json::parse(response.body.value());
    } else {
        return std::nullopt;
    }
}

std::vector<Repo> XptutoImpl::get_repos_sync(const User &user) {
    auto response = client->get_sync("https://api.github.com/users/" + user.login + "/repos");
    if (response.body && !std::empty(response.body.value())) {
        return nlohmann::json::parse(response.body.value());
    } else {
        return {};
    }
}
