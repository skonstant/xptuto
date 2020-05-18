//
// Created by stephane on 15/05/2020.
//
#ifdef EMSCRIPTEN

#include "web_http_client.hpp"
#include "http_response.hpp"

#include <emscripten/fetch.h>

using namespace xptuto;


void downloadSucceeded(emscripten_fetch_t *fetch) {
    if (WebHttpClient::callbacks.find(fetch->id) != WebHttpClient::callbacks.end()) {
        auto callback = WebHttpClient::callbacks[fetch->id];

        std::string_view body;
        if (fetch->data && fetch->numBytes > 0) {
            body = {fetch->data, static_cast<std::string_view::size_type>(fetch->numBytes)};
        }

        callback->on_response(body, fetch->status);

        const std::lock_guard<std::mutex> lock(WebHttpClient::callbacksMutex);
        WebHttpClient::callbacks.erase(fetch->id);
    }
    emscripten_fetch_close(fetch);
}

void downloadFailed(emscripten_fetch_t *fetch) {
    if (WebHttpClient::callbacks.find(fetch->id) != WebHttpClient::callbacks.end()) {
        auto callback = WebHttpClient::callbacks[fetch->id];
        if (fetch->status) {
            std::string_view body;
            if (fetch->data && fetch->numBytes > 0) {
                body = {fetch->data, static_cast<std::string_view::size_type>(fetch->numBytes)};
            }

            callback->on_response(body, fetch->status);
        } else {
            callback->on_failure("Download failed");
        }
        const std::lock_guard<std::mutex> lock(WebHttpClient::callbacksMutex);
        WebHttpClient::callbacks.erase(fetch->id);
    }
    emscripten_fetch_close(fetch);
}

void WebHttpClient::get(const std::string &url, const std::shared_ptr<HttpCallback> &callback) {
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;
    auto fetch = emscripten_fetch(&attr, url.c_str());
    const std::lock_guard<std::mutex> lock(callbacksMutex);
    callbacks.insert({fetch->id, callback});
}

HttpResponse WebHttpClient::get_sync(const std::string &url) {
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | EMSCRIPTEN_FETCH_SYNCHRONOUS;
    emscripten_fetch_t *fetch = emscripten_fetch(&attr, url.c_str()); // Blocks here until the operation is complete.

    std::optional<std::string> body;
    if (fetch->data && fetch->numBytes > 0) {
        body = {fetch->data, static_cast<std::string_view::size_type>(fetch->numBytes)};
    }

    std::optional<int32_t> code;
    if (fetch->status) {
        code = fetch->status;
    }
    auto response = HttpResponse(code, body);
    emscripten_fetch_close(fetch);
    return response;
}


#endif
