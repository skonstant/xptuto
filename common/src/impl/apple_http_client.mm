//
// Created by Stephane Konstantaropoulos on 20/06/18.
// Copyright (c) 2018 Stephane Konstantaropoulos. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "apple_http_client.hpp"
#import "DJIMarshal+Private.h"
#import "http_response.hpp"
#import "http_callback.hpp"

void ac::IOSHttp::get(const std::string &url, const std::optional<std::unordered_map<std::string, std::string>> &headers, const std::shared_ptr<ac::HttpCallback> &callback) {

    NSURLSession *session = [NSURLSession sharedSession];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[[NSURL alloc] initWithString:(::djinni::String::fromCpp(url))]];

    if (headers) {
        for (const auto &header : *headers) {
            [request addValue:(::djinni::String::fromCpp(header.second))
           forHTTPHeaderField:(::djinni::String::fromCpp(header.first))];
        }
    }

    __block auto cb = callback;

    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {

        NSString *body = nil;
        if (data) {
            body = [NSString stringWithUTF8String:static_cast<const char *>([data bytes])];
        }

        if (error) {
            cb->on_failure(::djinni::String::toCpp(error.localizedDescription));
        } else {
            NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
            int32_t code = [httpResponse statusCode];
            cb->on_response(HttpResponse(::djinni::String::toCpp(body), code));
        }
    }];

    [task resume];

}
