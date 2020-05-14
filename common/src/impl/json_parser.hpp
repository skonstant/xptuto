//
// Created by stephane on 14/05/2020.
//

#ifndef XPTUTO_JSON_PARSER_HPP
#define XPTUTO_JSON_PARSER_HPP

#include "../nlohmann/json.hpp"
#include "../gen/user.hpp"

using namespace xptuto;

namespace nlohmann {
    template<>
    struct adl_serializer<User> {
        static User from_json(const json &j) {
            return {j["login"].get<std::string>(), j["id"].get<int32_t>(),
                    j["avatar_url"].get<std::string>(),
                    std::chrono::system_clock::now()}; //TOOD parse time
        }

        static void to_json(json &j, User t) {
            throw std::invalid_argument("user is read only");
        }
    };
};

#endif //XPTUTO_JSON_PARSER_HPP
