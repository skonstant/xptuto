//
// Created by stephane on 13/05/2020.
//
#include "gtest/gtest.h"

#include <xptuto.hpp>
#include <user.hpp>
#include <repo.hpp>
#include "stubs/http_client_stub.hpp"
#include "stubs/platform_threads_stub.hpp"
#include <get_user_cb_impl.hpp>
#include <get_repos_cb_impl.hpp>
#include "thread_func_impl.hpp"
#include "xptuto_impl.hpp"
#include "sql_storage.hpp"
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
    EXPECT_FALSE(::xptuto::Xptuto::make_instance(stubHttp, stubThreads, std::nullopt) == nullptr);
}

TEST_F(Xptuto, GetUserTest) {
    auto instance = ::xptuto::Xptuto::make_instance(stubHttp, stubThreads, std::nullopt);

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
    auto instance = ::xptuto::Xptuto::make_instance(stubHttp, stubThreads, std::nullopt);
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
    auto instance = std::make_shared<XptutoImpl>(stubHttp, stubThreads, std::nullopt);
    stubHttp->path = "/responses/users_aosp.json";

    auto user = instance->get_user_sync("aosp");
    EXPECT_EQ("aosp", user->login);
}

TEST_F(Xptuto, GetRepoSyncTest) {
    auto instance = std::make_shared<XptutoImpl>(stubHttp, stubThreads, std::nullopt);
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

    stubThreads->create_thread("test", std::make_shared<ThreadFuncImpl>([p, id]() {
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

    stubThreads->run_on_main_thread(std::make_shared<ThreadFuncImpl>([p, id]() {
        EXPECT_EQ(id, std::this_thread::get_id());
        p->set_value();
    }));

    if (future.wait_for(1s) != std::future_status::ready) {
        FAIL();
    }
}

TEST_F(Xptuto, CheckSqliteTest) {
    XptutoImpl::check_sqlite3();
}

TEST_F(Xptuto, OrmTest) {
    auto instance = std::make_shared<XptutoImpl>(stubHttp, stubThreads, std::nullopt);
    stubHttp->path = "/responses/users_aosp.json";

    auto user = instance->get_user_sync("aosp");

    auto storage = SQLStorage(":memory:");
    storage.store_user(user.value());
    auto u = storage.get_user("aosp");
    EXPECT_EQ(u.created_at, user->created_at);
    EXPECT_EQ(u.login, "aosp");


    stubHttp->path = "/responses/repos_aosp.json";
    auto repos = instance->get_repos_sync(user.value());
    storage.store_repos(repos);
    auto reposList = storage.get_repos("aosp");
    EXPECT_EQ(reposList.size(), repos.size());
}
