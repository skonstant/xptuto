//
// Created by stephane on 15/05/2020.
//

#ifdef EMSCRIPTEN

#ifndef XPTUTO_WEBHTTPCLIENT_HPP
#define XPTUTO_WEBHTTPCLIENT_HPP

#include "http_client.hpp"
#include "http_callback.hpp"

#include <unordered_map>

class WebHttpClient : public xptuto::HttpClient {

public:
    void get(const std::string &url, const std::shared_ptr<xptuto::HttpCallback> &callback) override;

    static inline std::unordered_map<unsigned int, std::shared_ptr<xptuto::HttpCallback>> callbacks = {};
};

#endif //XPTUTO_WEBHTTPCLIENT_HPP

#endif