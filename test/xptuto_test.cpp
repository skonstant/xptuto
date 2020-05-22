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

#include "sqlite_orm.h"

TEST_F(Xptuto, OrmTest) {

    using namespace sqlite_orm;
    using namespace xptuto;
    auto storage = make_storage(":memory:",
                                make_table("users",
                                           make_column("id", &User::id, unique()),
                                           make_column("login", &User::login, unique()),
                                           make_column("avatar_url", &User::avatar_url),
                                           make_column("created_at", &User::created_at)),
                                make_table("repos",
                                           make_column("id", &Repo::id, unique()),
                                           make_column("name", &Repo::name),
                                           make_column("full_name", &Repo::full_name),
                                           make_column("owner", &Repo::owner),
                                           make_column("priv", &Repo::priv),
                                           make_column("descr", &Repo::descr),
                                           make_column("created_at", &Repo::created_at),
                                           foreign_key(&Repo::owner).references(&User::id)));
    storage.sync_schema();

    auto instance = std::make_shared<XptutoImpl>(stubHttp, stubThreads);
    stubHttp->path = "/responses/users_aosp.json";

    auto user = instance->get_user_sync("aosp");
    auto insertedId = storage.insert(*user);
    auto allUsersList = storage.get_all<User, std::vector<User>>(where(c(&User::login) == "aosp"));
    EXPECT_EQ(storage.count<User>(), 1);
    EXPECT_EQ(allUsersList[0].created_at, user->created_at);

}
