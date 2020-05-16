//
// Created by stephane on 14/05/2020.
//

#include "get_repos_cb_impl.hpp"

#include <utility>


GetReposCbImpl::GetReposCbImpl(std::function<void(const std::vector<xptuto::Repo> &, const xptuto::User &)> successFn,
                               std::function<void(const std::string &)> failureFn) : successFn(std::move(successFn)),
                                                                                     failureFn(std::move(failureFn)) {}

void GetReposCbImpl::on_success(const std::vector<xptuto::Repo> &repos, const xptuto::User &user) {
    successFn(repos, user);
}

void GetReposCbImpl::on_error(const std::string &error) {
    failureFn(error);
}