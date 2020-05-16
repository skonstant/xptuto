//
// Created by stephane on 14/05/2020.
//

#ifndef XPTUTO_GET_USER_CB_IMPL_HPP
#define XPTUTO_GET_USER_CB_IMPL_HPP

#include "get_user_cb.hpp"

#include <functional>

class GetUserCbImpl : public xptuto::GetUserCb {
public:

    GetUserCbImpl(std::function<void(const xptuto::User &)> successFn,
                  std::function<void(const std::string &)> failureFn);

    void on_success(const xptuto::User &user) override;

    void on_error(const std::string &error) override;

private:
    const std::function<void(const xptuto::User &)> successFn;
    const std::function<void(const std::string &)> failureFn;
};


#endif //XPTUTO_GET_USER_CB_IMPL_HPP
