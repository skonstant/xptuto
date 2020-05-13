//
// Created by stephane on 13/05/2020.
//
#include "gtest/gtest.h"

#include <xptuto.hpp>
#include <user.hpp>
#include <repo.hpp>

TEST(Xptuto, MakeInstanceTest) {
    EXPECT_FALSE(::xptuto::Xptuto::make_instance() == nullptr);
}

TEST(Xptuto, GetUsersEmptyTest) {
    auto instance = ::xptuto::Xptuto::make_instance();
    auto users = instance->get_users();
    EXPECT_FALSE( users.empty());
}

TEST(Xptuto, GetUsersCountTest) {
    auto instance = ::xptuto::Xptuto::make_instance();
    auto users = instance->get_users();
    EXPECT_EQ( users.size(), 2);
}

TEST(Xptuto, GetUsersNameTest) {
    auto instance = ::xptuto::Xptuto::make_instance();
    auto users = instance->get_users();
    EXPECT_EQ("Stéphane",users.front().name);
}


TEST(Xptuto, GetReposEmptyTest) {
    auto instance = ::xptuto::Xptuto::make_instance();
    auto users = instance->get_users();
    auto repos = instance->get_repos_for_user(users.front());
    EXPECT_TRUE(repos.empty());
}



