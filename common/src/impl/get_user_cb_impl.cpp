//
// Created by stephane on 14/05/2020.
//

#include "get_user_cb_impl.hpp"

#include <utility>

GetUserCbImpl::GetUserCbImpl(std::function<void(const xptuto::User &)> successFn,
                             std::function<void(const std::string &)> failureFn) : successFn(std::move(successFn)),
                                                                                   failureFn(std::move(failureFn)) {}

void GetUserCbImpl::on_success(const xptuto::User &user) {
    successFn(user);
}

void GetUserCbImpl::on_error(const std::string &error) {
    failureFn(error);
}