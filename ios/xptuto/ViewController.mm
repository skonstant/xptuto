//
//  ViewController.m
//  xptuto
//
//  Created by Stephane Konstantaropoulos on 02/05/2020.
//  Copyright © 2020 example. All rights reserved.
//

#import "ViewController.h"

#include "xptuto.hpp"
#include "user.hpp"
#include "repo.hpp"
#include "get_user_cb_impl.hpp"
#include "get_repos_cb_impl.hpp"
#include "apple_http_client.hpp"

using namespace xptuto;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    auto x = Xptuto::make_instance(std::make_shared<AppleHttpClient>());
    x->get_user("aosp", std::make_shared<GetUserCbImpl>(
            [x](const User &user) {
                NSLog(@"Got user with login: %s", user.login.c_str());

                x->get_repos_for_user(user, std::make_shared<GetReposCbImpl>(
                        [](const std::vector <Repo> &repos, const User &user) {
                            for (const auto &repo : repos) {
                                NSLog(@"Got repo with name: %s", repo.name.c_str());
                            }

                        }, [](const std::string &error) {
                            NSLog(@"error: %s", error.c_str());
                        }));
            }, [](const std::string &error) {
                NSLog(@"error: %s", error.c_str());
            }));
}


@end
