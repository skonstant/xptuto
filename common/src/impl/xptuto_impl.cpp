//
// Created by Stephane Konstantaropoulos on 11/05/2020.
//

#include "xptuto_impl.hpp"

#include <utility>
#include "http_client.hpp"
#include "http_callback_impl.hpp"
#include "get_users_cb.hpp"
#include "get_repos_cb.hpp"
#include "get_user_cb.hpp"
#include "json_parser.hpp"

using namespace xptuto;
using namespace std::chrono_literals;

std::shared_ptr<Xptuto> xptuto::Xptuto::make_instance(const std::shared_ptr<HttpClient> &client) {
    return std::make_shared<XptutoImpl>(client);
}

XptutoImpl::XptutoImpl(std::shared_ptr<xptuto::HttpClient> cl) : client(std::move(cl)) {}

void XptutoImpl::get_users(const std::shared_ptr<GetUsersCb> &cb) {
    // no local users fror now
    cb->on_success({});
}

void XptutoImpl::get_repos_for_user(const User &user, const std::shared_ptr<GetReposCb> &cb) {
    client->get("https://api.github.com/users/" + user.login + "/repos",
                std::make_shared<HttpCallbackImpl>([cb, user](const std::string_view &body, int32_t code) {
                    if (!std::empty(body)) {
                        std::vector<Repo> repos = nlohmann::json::parse(body);
                        cb->on_success(repos, user);
                    } else {
                        cb->on_error("error"); //TODO check HTTP code
                    }
                }, [cb](const std::string &) {
                    cb->on_error("error");
                }));
}

void XptutoImpl::get_user(const std::string &login, const std::shared_ptr<GetUserCb> &cb) {
    client->get("https://api.github.com/users/" + login,
                std::make_shared<HttpCallbackImpl>([cb](const std::string_view &body, int32_t code) {
                    if (!std::empty(body)) {
                        User user = nlohmann::json::parse(body);
                        cb->on_success(user);
                    } else {
                        cb->on_error("error"); //TODO check HTTP code
                    }
                }, [cb](const std::string &) {
                    cb->on_error("error");
                }));
}
