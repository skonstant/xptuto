//
// Created by stephane on 14/05/2020.
//

#ifndef XPTUTO_GET_REPOS_CB_IMPL_HPP
#define XPTUTO_GET_REPOS_CB_IMPL_HPP

#include "get_repos_cb.hpp"

#include <functional>

class GetReposCbImpl : public xptuto::GetReposCb {
public:

    GetReposCbImpl(std::function<void(const std::vector<xptuto::Repo> &, const xptuto::User &)> successFn,
                   std::function<void(const std::string &)> failureFn);

    void on_success(const std::vector<xptuto::Repo> &repos, const xptuto::User &) override;

    void on_error(const std::string &error) override;

private:
    const std::function<void(const std::vector<xptuto::Repo> &, const xptuto::User &)> successFn;
    const std::function<void(const std::string &)> failureFn;
};


#endif //XPTUTO_GET_REPOS_CB_IMPL_HPP
