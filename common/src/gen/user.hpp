// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from main.djinni

#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <utility>

namespace xptuto {

struct User final {
    std::string login;
    int32_t id;
    std::string avatar_url;
    std::chrono::system_clock::time_point created_at;

    User() = default;

    User(std::string login_,
         int32_t id_,
         std::string avatar_url_,
         std::chrono::system_clock::time_point created_at_)
    : login(std::move(login_))
    , id(std::move(id_))
    , avatar_url(std::move(avatar_url_))
    , created_at(std::move(created_at_))
    {}
};

}  // namespace xptuto
