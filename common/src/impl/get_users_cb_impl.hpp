//
// Created by stephane on 14/05/2020.
//

#ifndef XPTUTO_GET_USERS_CB_IMPL_HPP
#define XPTUTO_GET_USERS_CB_IMPL_HPP

#include "get_users_cb.hpp"

#include <functional>

class GetUsersCbImpl : public xptuto::GetUsersCb {
public:

    GetUsersCbImpl(std::function<void(const std::vector<xptuto::User> &)> successFn,
                   std::function<void(const std::string &)> failureFn);

    void on_success(const std::vector<xptuto::User> &users) override;

    void on_error(const std::string &error) override;

private:
    const std::function<void(const std::vector<xptuto::User> &)> successFn;
    const std::function<void(const std::string &)> failureFn;
};


#endif //XPTUTO_GET_USERS_CB_IMPL_HPP
