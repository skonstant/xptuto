//
// Created by stephane on 13/05/2020.
//
#include "gtest/gtest.h"

#include <xptuto.hpp>
#include <user.hpp>
#include "stubs/http_client_stub.hpp"
#include <get_users_cb_impl.hpp>
#include "http_response.hpp"

#include <future>

using namespace std::chrono_literals;

class Xptuto : public ::testing::Test {
protected:
    void SetUp() override {
        stubHttp = std::make_shared<HttpClientStub>();
        promise = std::make_shared<std::promise<void>>();
        future = promise->get_future();
    }

    std::shared_ptr<HttpClientStub> stubHttp;
    std::shared_ptr<std::promise<void>> promise;
    std::future<void> future;
};


TEST_F(Xptuto, MakeInstanceTest) {
    EXPECT_FALSE(::xptuto::Xptuto::make_instance(stubHttp) == nullptr);
}

TEST_F(Xptuto, GetUsersEmptyTest) {
    auto instance = ::xptuto::Xptuto::make_instance(stubHttp);

    stubHttp->path = "/responses/users_aosp.json";

    auto p = promise;

    instance->get_users(std::make_shared<GetUsersCbImpl>(
            [p](const std::vector<xptuto::User> &users) {
                EXPECT_FALSE(users.empty());
                p->set_value();
            }, [p](const std::string &) {
                FAIL();
            }
    ));
    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, GetUsersCountTest) {
    auto instance = ::xptuto::Xptuto::make_instance(stubHttp);

    stubHttp->path = "/responses/users_skonstan.json";

    auto p = promise;

    instance->get_users(std::make_shared<GetUsersCbImpl>(
            [p](const std::vector<xptuto::User> &users) {
                EXPECT_FALSE(users.empty());
                p->set_value();
            }, [p](const std::string &) {
                FAIL();
            }
    ));
    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, GetUsersNameTest) {
    auto instance = ::xptuto::Xptuto::make_instance(stubHttp);
    stubHttp->path = "/responses/users_skonstan.json";

    auto p = promise;

    instance->get_users(std::make_shared<GetUsersCbImpl>(
            [p](const std::vector<xptuto::User> &users) {
                EXPECT_EQ("sKonstan", users.front().login);
                p->set_value();
            }, [p](const std::string &) {
                FAIL();
            }
    ));
    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}



