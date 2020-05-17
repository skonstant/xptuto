//
// Created by stephane on 14/05/2020.
//

#ifndef XPTUTO_HTTP_CLIENT_STUB_HPP
#define XPTUTO_HTTP_CLIENT_STUB_HPP

#include "http_client.hpp"
#include "http_callback.hpp"
#include "http_response.hpp"

#include <optional>

class HttpClientStub : public xptuto::HttpClient {
public:
    void get(const std::string &url, const std::shared_ptr<xptuto::HttpCallback> &callback) override;

    xptuto::HttpResponse get_sync(const std::string &url) override;

    std::optional<std::string> path;
};


#endif //XPTUTO_HTTP_CLIENT_STUB_HPP
