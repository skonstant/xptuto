//
// Created by Stephane Konstantaropoulos.
// Copyright (c) 2020 Stephane Konstantaropoulos. All rights reserved.
//


#ifndef XPTUTO_APPLEHTTPCLIENT_HPP
#define XPTUTO_APPLEHTTPCLIENT_HPP

#ifdef __APPLE__

#include "http_client.hpp"
#include "http_callback.hpp"

#include <unordered_map>

class AppleHttpClient : public xptuto::HttpClient {

public:
    void get(const std::string &url, const std::shared_ptr<xptuto::HttpCallback> &callback) override;

    xptuto::HttpResponse get_sync(const std::string &url) override;
};

#endif

#endif //XPTUTO_APPLEHTTPCLIENT_HPP
