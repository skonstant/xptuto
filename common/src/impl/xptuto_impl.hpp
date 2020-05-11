//
// Created by Stephane Konstantaropoulos on 11/05/2020.
//

#ifndef XPTUTO_XPTUTO_IMPL_HPP
#define XPTUTO_XPTUTO_IMPL_HPP

#include "xptuto.hpp"
#include "user.hpp"
#include "repo.hpp"

class XptutoImpl : public xptuto::Xptuto {

    std::vector<xptuto::User> get_users() override;

    std::vector<xptuto::Repo> get_repos_for_user(const xptuto::User & u) override;

};


#endif //XPTUTO_XPTUTO_IMPL_HPP
