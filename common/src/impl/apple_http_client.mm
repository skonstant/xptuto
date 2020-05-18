//
// Created by Stephane Konstantaropoulos.
// Copyright (c) 2020 Stephane Konstantaropoulos. All rights reserved.
//

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#import "DJIMarshal+Private.h"

#include "apple_http_client.hpp"
#include "http_response.hpp"

using namespace djinni;

void AppleHttpClient::get(const std::string &url, const std::shared_ptr<xptuto::HttpCallback> &callback) {

    auto session = [NSURLSession sharedSession];
    auto u = [NSURL URLWithString:(String::fromCpp(url))];

    __block auto cb = callback;

    auto task = [session dataTaskWithURL:u completionHandler:^(NSData *data,
                                                               NSURLResponse *response,
                                                               NSError *error) {

        std::string_view body;

        if (data && data.length) {
            body = {(char *) [data bytes], data.length};
        }

        if (error) {
            cb->on_failure(String::toCpp(error.localizedDescription));
        } else {
            auto httpResponse = (NSHTTPURLResponse *) response;
            cb->on_response(body, static_cast<int32_t>(httpResponse.statusCode));
        }
    }];

    [task resume];

}

xptuto::HttpResponse AppleHttpClient::get_sync(const std::string &url) {
    auto request = [NSURLRequest requestWithURL:[NSURL URLWithString:(String::fromCpp(url))]];

    NSHTTPURLResponse *response;
    NSError *error = nil;
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];

    if (error) {
        throw std::runtime_error(String::toCpp(error.localizedDescription));
    }

    std::optional<std::string> body;
    std::optional<int32_t> code;

    if (data && data.length) {
        body = std::make_optional<std::string>((char *) [data bytes], data.length);
    }
    if (response) {
        code = response.statusCode;
    }

    return {code, body};

}

#endif
