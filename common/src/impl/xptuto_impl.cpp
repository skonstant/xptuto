//
// Created by Stephane Konstantaropoulos on 11/05/2020.
//

#include "xptuto_impl.hpp"

#include <utility>
#include "http_client.hpp"
#include "http_callback_impl.hpp"
#include "get_users_cb.hpp"
#include "json_parser.hpp"

using namespace xptuto;
using namespace std::chrono_literals;

std::shared_ptr<Xptuto> xptuto::Xptuto::make_instance(const std::shared_ptr<HttpClient> &client) {
    return std::make_shared<XptutoImpl>(client);
}

XptutoImpl::XptutoImpl(std::shared_ptr<xptuto::HttpClient> cl) : client(std::move(cl)) {}

void XptutoImpl::get_users(const std::shared_ptr<GetUsersCb> &cb) {
    client->get("https://api.github.com/users/aosp",
                std::make_shared<HttpCallbackImpl>([cb](const std::string_view &body, int32_t code) {
                    if (!std::empty(body)) {
                        User user = nlohmann::json::parse(body);
                        cb->on_success({user});
                    } else {
                        cb->on_error("error"); //TODO check HTTP code
                    }
                }, [cb](const std::string &) {
                    cb->on_error("error");
                }));
}

void XptutoImpl::get_repos_for_user(const std::shared_ptr<GetReposCb> &cb) {

}
