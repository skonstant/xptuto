//
// Created by stephane on 14/05/2020.
//

#include "http_client_stub.hpp"
#include <thread>
#include <utility>
#include <fstream>

#ifndef TEST_PATH
#error "test path not defined"
#endif

using namespace xptuto;

void HttpClientStub::get(const std::string &url, const std::shared_ptr<HttpCallback> &callback) {
    auto p = path.value();

    std::thread t([p](const std::shared_ptr<xptuto::HttpCallback> &cb) {
        auto stream = std::ifstream(std::string(TEST_PATH) + p);
        auto response = std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        cb->on_response(response, 200);
    }, callback);
    t.detach();
}

HttpResponse HttpClientStub::get_sync(const std::string &url) {
    auto p = path.value();
    auto stream = std::ifstream(std::string(TEST_PATH) + p);
    auto response = std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return {200, response};
}
