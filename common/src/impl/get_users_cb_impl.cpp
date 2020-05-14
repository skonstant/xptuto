//
// Created by stephane on 14/05/2020.
//

#include "get_users_cb_impl.hpp"

#include <utility>


GetUsersCbImpl::GetUsersCbImpl(std::function<void(const std::vector<xptuto::User> &)> successFn,
                               std::function<void(const std::string &)> failureFn) : successFn(std::move(successFn)),
                                                                                            failureFn(std::move(failureFn)) {}

void GetUsersCbImpl::on_success(const std::vector<xptuto::User> &users) {
    successFn(users);
}

void GetUsersCbImpl::on_error(const std::string &error) {
    failureFn(error);
}