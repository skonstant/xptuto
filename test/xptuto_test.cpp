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
#include "thread_func_impl.hpp"
#include "web_threads.hpp"
#include "xptuto_impl.hpp"

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

    if (future.wait_for(2s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, GetUserSyncTest) {
    auto instance = std::make_shared<XptutoImpl>(stubHttp, stubThreads);
    stubHttp->path = "/responses/users_aosp.json";

    auto user = instance->get_user_sync("aosp");
    EXPECT_EQ("aosp", user->login);
}

TEST_F(Xptuto, GetRepoSyncTest) {
    auto instance = std::make_shared<XptutoImpl>(stubHttp, stubThreads);
    stubHttp->path = "/responses/users_aosp.json";

    auto user = instance->get_user_sync("aosp");
    EXPECT_EQ("aosp", user->login);

    stubHttp->path = "/responses/repos_aosp.json";
    auto repos = instance->get_repos_sync(user.value());
    EXPECT_FALSE(repos.empty());
}


TEST_F(Xptuto, BackgroundThreadTest) {
    auto p = promise;

    auto id = std::this_thread::get_id();

    stubThreads->create_thread("test", std::make_shared<ThreadFuncImpl>([p, id](){
        EXPECT_NE(id, std::this_thread::get_id());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, MainThreadTest) {
    auto p = promise;

    auto id = std::this_thread::get_id();

    stubThreads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([p, id](){
        EXPECT_EQ(id, std::this_thread::get_id());
        p->set_value();
    }));

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

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, WebGetUserSyncTest) {
    auto webThreads = std::make_shared<WebThreads>();

    auto p = promise;

    webThreads->create_thread("WebGetUserSyncTest", std::make_shared<ThreadFuncImpl>([p](){
        auto webHttp = std::make_shared<WebHttpClient>();
        auto response = webHttp->get_sync("https://api.github.com/users/aosp");
        EXPECT_EQ(response.code.value(), 200);
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }

}

TEST_F(Xptuto, WebGet404SyncTest) {
    auto webThreads = std::make_shared<WebThreads>();

    auto p = promise;

    webThreads->create_thread("WebGet404SyncTest", std::make_shared<ThreadFuncImpl>([p](){
        auto webHttp = std::make_shared<WebHttpClient>();
        auto response = webHttp->get_sync("https://api.github.com/users/aospppp");
        EXPECT_EQ(response.code.value(), 404);
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, WebBackgroundThreadTest) {
    auto webThreads = std::make_shared<WebThreads>();
    auto p = promise;

    auto id = std::this_thread::get_id();

    webThreads->create_thread("test", std::make_shared<ThreadFuncImpl>([p, id](){
        EXPECT_NE(id, std::this_thread::get_id());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, WebMainThreadTest) {
    auto webThreads = std::make_shared<WebThreads>();
    auto p = promise;

    webThreads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([p, webThreads](){
        EXPECT_TRUE(webThreads->is_main_thread());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}


TEST_F(Xptuto, GetUserAndReposTest) {
    auto webThreads = std::make_shared<WebThreads>();
    auto instance = ::xptuto::Xptuto::make_instance(std::make_shared<WebHttpClient>(), webThreads);

    auto p = promise;

    instance->get_repos_for_user_name("aosp", std::make_shared<GetReposCbImpl>(
            [p, webThreads](const std::vector<xptuto::Repo> &repos,const xptuto::User &user) {
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



#elif __APPLE__

#include "apple_http_client.hpp"
#include "http_callback_impl.hpp"
#include "apple_threads.hpp"

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

TEST_F(Xptuto, AppleGetUserSyncTest) {
    auto webHttp = std::make_shared<AppleHttpClient>();
    auto response = webHttp->get_sync("https://api.github.com/users/aosp");
    EXPECT_EQ(response.code.value(), 200);
}

TEST_F(Xptuto, AppleGet404SyncTest) {
    auto webHttp = std::make_shared<AppleHttpClient>();
    auto response = webHttp->get_sync("https://api.github.com/users/aospppp");
    EXPECT_EQ(response.code.value(), 404);
}

TEST_F(Xptuto, AppleBackgroundThreadTest) {
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

TEST_F(Xptuto, AppleMainThreadTest) {
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



