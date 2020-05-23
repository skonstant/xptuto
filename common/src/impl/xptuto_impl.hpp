//
// Created by Stephane Konstantaropoulos on 11/05/2020.
//

#ifndef XPTUTO_XPTUTO_IMPL_HPP
#define XPTUTO_XPTUTO_IMPL_HPP

#include "xptuto.hpp"
#include "user.hpp"
#include "repo.hpp"
#include "sql_storage.hpp"
#include <vector>

class XptutoImpl : public xptuto::Xptuto, public std::enable_shared_from_this<XptutoImpl> {

public:
    explicit XptutoImpl(std::shared_ptr<xptuto::HttpClient> client, std::shared_ptr<xptuto::PlatformThreads> threads);

    std::optional<xptuto::User> get_user_sync(const std::string &);

    std::vector<xptuto::Repo> get_repos_sync(const xptuto::User &);

    static void check_sqlite3();

    static inline std::shared_ptr<XptutoImpl> instance = nullptr;
private:

    void get_user(const std::string &login, const std::shared_ptr<xptuto::GetUserCb> &cb) override;

    void get_users(const std::shared_ptr<xptuto::GetUsersCb> &cb) override;

    void get_repos_for_user(const xptuto::User &usr, const std::shared_ptr<xptuto::GetReposCb> &cb) override;

    void get_repos_for_user_name(const std::string &username, const std::shared_ptr<xptuto::GetReposCb> &cb) override;

    const std::shared_ptr<xptuto::HttpClient> client;
    const std::shared_ptr<xptuto::PlatformThreads> threads;

    SQLStorage storage;
};


#endif //XPTUTO_XPTUTO_IMPL_HPP
