//
// Created by Stephane Konstantaropoulos.
// Copyright (c) 2020 Stephane Konstantaropoulos. All rights reserved.
//

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#import "DJIMarshal+Private.h"
#import "apple_http_client.hpp"

using namespace djinni;

void AppleHttpClient::get(const std::string &url, const std::shared_ptr<xptuto::HttpCallback> &callback) {

    NSURLSession *session = [NSURLSession sharedSession];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[[NSURL alloc] initWithString:(String::fromCpp(
            url))]];

    __block auto cb = callback;

    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData *data,
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

#endif
