//
// Created by stephane on 23/05/2020.
//

#ifndef XPTUTO_SQL_STORAGE_HPP
#define XPTUTO_SQL_STORAGE_HPP

#include "../sqlite_orm/sqlite_orm.h"
#include "user.hpp"
#include "repo.hpp"

/**
 *  Specialize sqlite_orm for time_point
 */
namespace sqlite_orm {

    template<>
    struct type_printer<std::chrono::system_clock::time_point, void> : public integer_printer {
    };

    template<>
    struct statement_binder<std::chrono::system_clock::time_point> {
        int bind(sqlite3_stmt *stmt, int index, const std::chrono::system_clock::time_point &value) {
            auto val = std::chrono::duration_cast<std::chrono::milliseconds>(value.time_since_epoch()).count();
            return sqlite3_bind_int64(stmt, index, static_cast<sqlite3_int64>(val));
        }
    };

    template<>
    struct field_printer<std::chrono::system_clock::time_point> {
        std::string operator()(const std::chrono::system_clock::time_point &t) const {
            std::stringstream stream;
            stream << std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count();
            return stream.str();
        }
    };

    template<>
    struct row_extractor<std::chrono::system_clock::time_point> {
        std::chrono::system_clock::time_point extract(const char *row_value) {
            auto millis = std::stoll(row_value);
            std::chrono::system_clock::time_point tp;
            return tp + std::chrono::milliseconds(millis);
        }

        std::chrono::system_clock::time_point extract(sqlite3_stmt *stmt, int columnIndex) {
            auto millis = sqlite3_column_int64(stmt, columnIndex);
            std::chrono::system_clock::time_point tp;
            return tp + std::chrono::milliseconds(millis);
        }
    };

}


inline auto initStorage(const std::string &dbFile) {
    using namespace sqlite_orm;
    using namespace xptuto;

    return make_storage(dbFile,
                        make_table("users",
                                   make_column("id", &User::id, primary_key()),
                                   make_column("login", &User::login, unique()),
                                   make_column("avatar_url", &User::avatar_url),
                                   make_column("created_at", &User::created_at)),
                        make_table("repos",
                                   make_column("id", &Repo::id, primary_key()),
                                   make_column("name", &Repo::name),
                                   make_column("full_name", &Repo::full_name),
                                   make_column("owner", &Repo::owner),
                                   make_column("priv", &Repo::priv),
                                   make_column("descr", &Repo::descr),
                                   make_column("created_at", &Repo::created_at),
                                   foreign_key(&Repo::owner).references(&User::id)));
}

using Storage = decltype(initStorage(""));

class SQLStorage {
public:
    explicit SQLStorage(std::string dbFile);

    void store_user(const xptuto::User &user);

    void store_repos(const std::vector<xptuto::Repo> &repos);

    xptuto::User get_user(const std::string &login);

    std::vector<xptuto::Repo> get_repos(const std::string &userName);

private:
    const std::string dbFile;
    Storage storage;
};


#endif //XPTUTO_SQL_STORAGE_HPP
