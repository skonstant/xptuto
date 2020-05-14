//
// Created by Stephane Konstantaropoulos on 11/05/2020.
//

#ifndef XPTUTO_XPTUTO_IMPL_HPP
#define XPTUTO_XPTUTO_IMPL_HPP

#include "xptuto.hpp"
#include "user.hpp"
#include "repo.hpp"

class XptutoImpl : public xptuto::Xptuto , public std::enable_shared_from_this<XptutoImpl> {

public:
    explicit XptutoImpl(std::shared_ptr<xptuto::HttpClient>  client);

private:

    void get_users(const std::shared_ptr<xptuto::GetUsersCb> & cb) override;

    void get_repos_for_user(const std::shared_ptr<xptuto::GetReposCb> & cb) override;

private:
    const std::shared_ptr<xptuto::HttpClient> client;

};


#endif //XPTUTO_XPTUTO_IMPL_HPP
