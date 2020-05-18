//
// Created by stephane on 13/05/2020.
//
#if __APPLE__

#include "apple_http_client.hpp"
#include "http_callback_impl.hpp"
#include "apple_threads.hpp"

#include "gtest/gtest.h"

#include <xptuto.hpp>
#include <user.hpp>
#include <repo.hpp>
#include <get_users_cb_impl.hpp>
#include <get_user_cb_impl.hpp>
#include <get_repos_cb_impl.hpp>
#include "thread_func_impl.hpp"
#include "xptuto_impl.hpp"

#include <future>

using namespace std::chrono_literals;

class AppleXptuto : public ::testing::Test {
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


TEST_F(AppleXptuto, AppleGetAsyncTest) {
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

TEST_F(AppleXptuto, AppleGetUserSyncTest) {
    auto webHttp = std::make_shared<AppleHttpClient>();
    auto response = webHttp->get_sync("https://api.github.com/users/aosp");
    EXPECT_EQ(response.code.value(), 200);
}

TEST_F(AppleXptuto, AppleGet404SyncTest) {
    auto webHttp = std::make_shared<AppleHttpClient>();
    auto response = webHttp->get_sync("https://api.github.com/users/aospppp");
    EXPECT_EQ(response.code.value(), 404);
}

TEST_F(AppleXptuto, AppleBackgroundThreadTest) {
    auto appleThreads = std::make_shared<AppleThreads>();
    auto p = promise;

    auto id = std::this_thread::get_id();

    appleThreads->create_thread("test", std::make_shared<ThreadFuncImpl>([p, id](){
        EXPECT_NE(id, std::this_thread::get_id());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(AppleXptuto, AppleMainThreadTest) {
    auto appleThreads = std::make_shared<AppleThreads>();
    auto p = promise;

    appleThreads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([p, appleThreads](){
        EXPECT_TRUE(appleThreads->is_main_thread());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

#endif



