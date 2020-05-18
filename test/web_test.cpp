//
// Created by stephane on 13/05/2020.
//

#ifdef EMSCRIPTEN

#include "gtest/gtest.h"

#include <xptuto.hpp>
#include <user.hpp>
#include <repo.hpp>
#include <get_users_cb_impl.hpp>
#include <get_user_cb_impl.hpp>
#include <get_repos_cb_impl.hpp>
#include "thread_func_impl.hpp"
#include "web_threads.hpp"
#include "xptuto_impl.hpp"

#include <future>

#include "web_http_client.hpp"
#include "http_callback_impl.hpp"
#include "http_response.hpp"

using namespace std::chrono_literals;

class WebXptuto : public ::testing::Test {
protected:
    void SetUp() override {
        webHttp = std::make_shared<WebHttpClient>();
        webThreads = std::make_shared<WebThreads>();
        promise = std::make_shared<std::promise<void>>();
        future = promise->get_future();
    }

    std::shared_ptr<WebHttpClient> webHttp;
    std::shared_ptr<WebThreads> webThreads;
    std::shared_ptr<std::promise<void>> promise;
    std::future<void> future;
};

TEST_F(WebXptuto, WebGetAsyncTest) {
    auto p = promise;

    webHttp->get("https://api.github.com/users/aosp", std::make_shared<HttpCallbackImpl>(
            [p](const std::string_view &body, int32_t code) {
                EXPECT_EQ(code, 200);
                p->set_value();
            }, [p](const std::string &) {
                FAIL();
            }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(WebXptuto, WebGetSyncTest) {
    auto p = promise;
    auto http = webHttp;

    webThreads->create_thread("WebGetUserSyncTest", std::make_shared<ThreadFuncImpl>([p, http]() {
        auto response = http->get_sync("https://api.github.com/users/aosp");
        EXPECT_EQ(response.code.value(), 200);
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }

}

TEST_F(WebXptuto, WebGet404SyncTest) {
    auto p = promise;
    auto http = webHttp;

    webThreads->create_thread("WebGet404SyncTest", std::make_shared<ThreadFuncImpl>([p, http]() {
        auto response = http->get_sync("https://api.github.com/users/aospppp");
        EXPECT_EQ(response.code.value(), 404);
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(WebXptuto, WebBackgroundThreadTest) {
    auto p = promise;

    auto id = std::this_thread::get_id();

    webThreads->create_thread("test", std::make_shared<ThreadFuncImpl>([p, id]() {
        EXPECT_NE(id, std::this_thread::get_id());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(WebXptuto, WebMainThreadTest) {
    auto webThreads = std::make_shared<WebThreads>();
    auto p = promise;

    webThreads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([p, webThreads]() {
        EXPECT_TRUE(webThreads->is_main_thread());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}


TEST_F(WebXptuto, GetUserAndReposTest) {
    auto webThreads = std::make_shared<WebThreads>();
    auto instance = ::xptuto::Xptuto::make_instance(std::make_shared<WebHttpClient>(), webThreads);

    auto p = promise;

    instance->get_repos_for_user_name("aosp", std::make_shared<GetReposCbImpl>(
            [p, webThreads](const std::vector<xptuto::Repo> &repos, const xptuto::User &user) {
                EXPECT_EQ("aosp", user.login);
                EXPECT_FALSE(repos.empty());
                EXPECT_TRUE(webThreads->is_main_thread());
                p->set_value();
            }, [p](const std::string &) {
                FAIL();
            }
    ));

    if (future.wait_for(2s) != std::future_status::ready) {
        FAIL();
    }
}

#endif



