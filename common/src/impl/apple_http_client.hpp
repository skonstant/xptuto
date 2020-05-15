//
// Created by Stephane Konstantaropoulos on 20/06/18.
// Copyright (c) 2018 Stephane Konstantaropoulos. All rights reserved.
//

#import "http.hpp"

namespace ac {

    class IOSHttp : public Http {
    public:
        void get(const std::string &url, const std::optional<std::unordered_map<std::string, std::string>> &headers, const std::shared_ptr<HttpCallback> &callback) override;
    };
};
