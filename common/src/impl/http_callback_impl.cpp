//
// Created by stephane on 14/05/2020.
//

#include "http_callback_impl.hpp"

#include <utility>

HttpCallbackImpl::HttpCallbackImpl(std::function<void(const std::string_view & body, int32_t code)> successFn,
                                   std::function<void(const std::string &)> failureFn) : successFn(
        std::move(successFn)),
                                                                                         failureFn(std::move(
                                                                                                 failureFn)) {}

void HttpCallbackImpl::on_response(const std::string_view & body, int32_t code) {
    successFn(body, code);
}

void HttpCallbackImpl::on_failure(const std::string &reason) {
    failureFn(reason);
}

