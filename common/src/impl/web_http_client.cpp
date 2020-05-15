//
// Created by stephane on 15/05/2020.
//
#ifdef EMSCRIPTEN

#include "web_http_client.hpp"

using namespace xptuto;

#include <emscripten/fetch.h>

void downloadSucceeded(emscripten_fetch_t *fetch) {
    if (WebHttpClient::callbacks.find(fetch->id) != WebHttpClient::callbacks.end()) {
        auto callback = WebHttpClient::callbacks[fetch->id];

        std::optional<std::string> body;
        if (fetch->numBytes > 0) {
            std::string str;
            str.append(fetch->data, fetch->numBytes);
            body = str;
        }

        auto response = HttpResponse(body, fetch->status);
        callback->on_response(response);
        WebHttpClient::callbacks.erase(fetch->id);
    }
    emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch) {
    if (WebHttpClient::callbacks.find(fetch->id) != WebHttpClient::callbacks.end()) {
        auto callback = WebHttpClient::callbacks[fetch->id];
        if (fetch->status) {
            std::optional<std::string> body;
            if (fetch->numBytes > 0) {
                std::string str;
                str.append(fetch->data, fetch->numBytes);
                body = str;
            }

            auto response = HttpResponse(body, fetch->status);
            callback->on_response(response);
        } else {
            callback->on_failure("Download failed");
        }
        WebHttpClient::callbacks.erase(fetch->id);
    }
    emscripten_fetch_close(fetch); // Also free data on failure.
}

void WebHttpClient::get(const std::string &url, const std::shared_ptr<HttpCallback> &callback) {
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;
    auto fetch = emscripten_fetch(&attr, url.c_str());
    callbacks.insert({fetch->id, callback});
}


#endif
