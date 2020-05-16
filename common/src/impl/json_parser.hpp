//
// Created by stephane on 14/05/2020.
//

#ifndef XPTUTO_JSON_PARSER_HPP
#define XPTUTO_JSON_PARSER_HPP

#include "../nlohmann/json.hpp"
#include "user.hpp"
#include "repo.hpp"
#include "date.h"

using namespace xptuto;

date::sys_time<std::chrono::milliseconds>
parse8601(std::istream &&is) {
    std::string save;
    is >> save;
    std::istringstream in{save};
    date::sys_time<std::chrono::milliseconds> tp;
    in >> date::parse("%FT%TZ", tp);
    if (in.fail()) {
        in.clear();
        in.exceptions(std::ios::failbit);
        in.str(save);
        in >> date::parse("%FT%T%Ez", tp);
    }
    return tp;
}

template<class T>
inline std::optional<T> valOrNull(const nlohmann::json &j, const std::string &key) {
    return j.find(key) == j.end() || j[key].is_null() ? std::nullopt : std::optional<T>(
            j[key].get<T>());
}

namespace nlohmann {
    template<>
    struct adl_serializer<User> {
        static User from_json(const json &j) {
            return {j["login"].get<std::string>(), j["id"].get<int32_t>(),
                    j["avatar_url"].get<std::string>(),
                    parse8601(std::istringstream(j["created_at"].get<std::string>()))};
        }

        static void to_json(json &j, User t) {
            throw std::invalid_argument("user is read only");
        }
    };

    template<>
    struct adl_serializer<Repo> {
        static Repo from_json(const json &j) {
            return {j["id"].get<int32_t>(), j["name"].get<std::string>(),
                    j["full_name"].get<std::string>(),
                    j["owner"]["id"].get<int32_t>(),
                    j["private"].get<bool>(),
                    valOrNull<std::string>(j, "description"),
                    parse8601(std::istringstream(j["created_at"].get<std::string>()))};
        }

        static void to_json(json &j, Repo t) {
            throw std::invalid_argument("user is read only");
        }
    };
};

#endif //XPTUTO_JSON_PARSER_HPP
