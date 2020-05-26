//
// Created by stephane on 23/05/2020.
//

#include "sql_storage.hpp"

#include <utility>

using namespace sqlite_orm;
using namespace xptuto;

SQLStorage::SQLStorage(const std::string& dbFile) : storage(initStorage(dbFile)) {
    storage.sync_schema();
}

void SQLStorage::store_user(const User &user) {
    storage.replace(user);
}

void SQLStorage::store_repos(const std::vector<Repo> &repos) {
    auto &s = storage;
    storage.transaction([&s, &repos] {
        for (const auto &repo : repos) {
            s.replace(repo);
        }
        return true;
    });
}

User SQLStorage::get_user(const std::string &login) {
    auto allUsersList = storage.get_all<User, std::vector<User>>(where(c(&User::login) == login));
    return allUsersList.at(0);
}

std::vector<Repo> SQLStorage::get_repos(const std::string &userName) {
    auto allUsersList = storage.get_all<User, std::vector<User>>(where(c(&User::login) == userName));
    auto u = allUsersList.at(0);
    return storage.get_all<Repo, std::vector<Repo>>(where(c(&Repo::owner) == u.id));
}
