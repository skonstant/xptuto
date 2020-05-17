//
// Created by stephane on 13/05/2020.
//
#include "gtest/gtest.h"

#include <xptuto.hpp>
#include <user.hpp>
#include <repo.hpp>
#include "stubs/http_client_stub.hpp"
#include "stubs/platform_threads_stub.hpp"
#include <get_users_cb_impl.hpp>
#include <get_user_cb_impl.hpp>
#include <get_repos_cb_impl.hpp>

#include <future>

using namespace std::chrono_literals;

class Xptuto : public ::testing::Test {
protected:
    void SetUp() override {
        stubHttp = std::make_shared<HttpClientStub>();
        stubThreads = std::make_shared<PlatformThreadsStub>();
        promise = std::make_shared<std::promise<void>>();
        future = promise->get_future();
    }

    std::shared_ptr<HttpClientStub> stubHttp;
    std::shared_ptr<PlatformThreadsStub> stubThreads;
    std::shared_ptr<std::promise<void>> promise;
    std::future<void> future;
};


TEST_F(Xptuto, MakeInstanceTest) {
    EXPECT_FALSE(::xptuto::Xptuto::make_instance(stubHttp, stubThreads) == nullptr);
}

TEST_F(Xptuto, GetUserTest) {
    auto instance = ::xptuto::Xptuto::make_instance(stubHttp, stubThreads);

    stubHttp->path = "/responses/users_aosp.json";

    auto p = promise;

    instance->get_user("aosp", std::make_shared<GetUserCbImpl>(
            [p](const xptuto::User &user) {
                EXPECT_EQ("aosp", user.login);
                p->set_value();
            }, [p](const std::string &) {
                FAIL();
            }
    ));
    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, GetReposTest) {
    auto instance = ::xptuto::Xptuto::make_instance(stubHttp, stubThreads);
    stubHttp->path = "/responses/users_aosp.json";

    auto p = promise;
    auto stub = stubHttp;

    instance->get_user("aosp", std::make_shared<GetUserCbImpl>(
            [p, stub, instance](const xptuto::User &user) {
                stub->path = "/responses/repos_aosp.json";

                instance->get_repos_for_user(user, std::make_shared<GetReposCbImpl>(
                        [p](const std::vector<xptuto::Repo> &repos, const xptuto::User &user) {
                            EXPECT_EQ("aosp", user.login);
                            EXPECT_FALSE(repos.empty());
                            p->set_value();
                        }, [p](const std::string &) {
                            FAIL();
                        }
                ));


            }, [p](const std::string &) {
                FAIL();
            }
    ));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

#ifdef EMSCRIPTEN

#include "web_http_client.hpp"
#include "http_callback_impl.hpp"

TEST_F(Xptuto, WebGetUserTest) {
    auto webHttp = std::make_shared<WebHttpClient>();

    auto p = promise;

    webHttp->get("https://api.github.com/users/aosp", std::make_shared<HttpCallbackImpl>(
            [p](const std::string_view &body, int32_t code) {
                EXPECT_EQ(code, 200);
                p->set_value();
            }, [p](const std::string &) {
                FAIL();
            }));

    if (future.wait_for(10s) != std::future_status::ready) {
        FAIL();
    }
}

#elif __APPLE__

#include "apple_http_client.hpp"
#include "http_callback_impl.hpp"

TEST_F(Xptuto, AppleGetUserTest) {
    auto appleHttp = std::make_shared<AppleHttpClient>();

    auto p = promise;

    appleHttp->get("https://api.github.com/users/aosp", std::make_shared<HttpCallbackImpl>(
            [p](const std::string_view & body, int32_t code) {
                EXPECT_EQ(code, 200);
                p->set_value();
            }, [p](const std::string &) {
                FAIL();
            }));

    if (future.wait_for(10s) != std::future_status::ready) {
        FAIL();
    }
}

#endif



