//
// Created by stephane on 14/05/2020.
//

#ifndef XPTUTO_HTTP_CALLBACK_IMPL_HPP
#define XPTUTO_HTTP_CALLBACK_IMPL_HPP

#include "http_callback.hpp"
#include <functional>

class HttpCallbackImpl : public xptuto::HttpCallback {

public:
    HttpCallbackImpl(std::function<void(const std::string_view &body, int32_t code)> successFn,
                     std::function<void(const std::string &)> failureFn);

    void on_response(const std::string_view &body, int32_t code) override;

    void on_failure(const std::string &reason) override;

private:
    const std::function<void(const std::string_view &body, int32_t code)> successFn;
    const std::function<void(const std::string &reason)> failureFn;

};


#endif //XPTUTO_HTTP_CALLBACK_IMPL_HPP
