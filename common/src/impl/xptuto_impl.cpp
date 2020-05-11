//
// Created by Stephane Konstantaropoulos on 11/05/2020.
//

#include "xptuto_impl.hpp"

using namespace xptuto;
using namespace std::chrono_literals;

std::shared_ptr<Xptuto> xptuto::Xptuto::make_instance() {
    return std::make_shared<XptutoImpl>();
}

std::vector<User> XptutoImpl::get_users() {
    return {
            User("sKonstan",
            18196480,
            "https://avatars2.githubusercontent.com/u/18196480?v=4",
            "",
            "https://api.github.com/users/sKonstan",
            "https://github.com/sKonstan",
            "User",
            false,
            "Stéphane",
            "none",
            "easy rider",
            1,
            0,
            0,
            0,
            std::chrono::system_clock::now() - 10h,
                    std::chrono::system_clock::now() - 1h
            ),
            User(
                    "mchiasson",
            961637,
            "https://avatars1.githubusercontent.com/u/961637?v=4",
            "",
            "https://api.github.com/users/mchiasson",
            "https://github.com/mchiasson",
            "User",
            false,
            "Matt Chiasson",
            "Canopy Growth",
            "",
            63,
            0,
            5,
            0,
                    std::chrono::system_clock::now() - 15h,
                    std::chrono::system_clock::now() - 3h
            )
    };
}

std::vector<Repo> XptutoImpl::get_repos_for_user(const User & u) {
    return {};
}