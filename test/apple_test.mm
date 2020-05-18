//
// Created by stephane on 13/05/2020.
//
#if __APPLE__

#import <Foundation/Foundation.h>

#include "apple_http_client.hpp"
#include "http_callback_impl.hpp"
#include "apple_threads.hpp"
#include "http_response.hpp"

#include "gtest/gtest.h"

#include "thread_func_impl.hpp"

#include <future>

using namespace std::chrono_literals;

class AppleXptuto : public ::testing::Test {
protected:
    void SetUp() override {
        appleHttp = std::make_shared<AppleHttpClient>();
        appleThreads = std::make_shared<AppleThreads>();
        promise = std::make_shared<std::promise<void>>();
        future = promise->get_future();
    }

    std::shared_ptr<AppleHttpClient> appleHttp;
    std::shared_ptr<AppleThreads> appleThreads;
    std::shared_ptr<std::promise<void>> promise;
    std::future<void> future;
};


TEST_F(AppleXptuto, AppleGetAsyncTest) {
    auto p = promise;

    appleHttp->get("https://api.github.com/users/aosp", std::make_shared<HttpCallbackImpl>(
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

TEST_F(AppleXptuto, AppleGetUserSyncTest) {
    auto response = appleHttp->get_sync("https://api.github.com/users/aosp");
    EXPECT_EQ(response.code.value(), 200);
}

TEST_F(AppleXptuto, AppleGet404SyncTest) {
    auto response = appleHttp->get_sync("https://api.github.com/users/aospppp");
    EXPECT_EQ(response.code.value(), 404);
}

TEST_F(AppleXptuto, AppleBackgroundThreadTest) {
    auto p = promise;

    auto id = std::this_thread::get_id();

    appleThreads->create_thread("test", std::make_shared<ThreadFuncImpl>([p, id]() {
        EXPECT_NE(id, std::this_thread::get_id());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(AppleXptuto, AppleMainThreadTest) {
    auto p = promise;

    appleThreads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([p]() {
        EXPECT_TRUE(NSThread.isMainThread);
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

#endif



