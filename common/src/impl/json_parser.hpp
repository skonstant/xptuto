//
// Created by stephane on 14/05/2020.
//

#ifndef XPTUTO_JSON_PARSER_HPP
#define XPTUTO_JSON_PARSER_HPP

#include "../nlohmann/json.hpp"
#include "../gen/user.hpp"
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
};

#endif //XPTUTO_JSON_PARSER_HPP
