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
#include "get_users_cb_impl.hpp"
#include "apple_http_client.hpp"

using namespace xptuto;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    auto x = Xptuto::make_instance(std::make_shared<AppleHttpClient>());
    x->get_users(std::make_shared<GetUsersCbImpl>(
            [](const std::vector <xptuto::User> &users) {
                for (const auto &user : users) {
                    NSLog(@"Got user with login: %s", user.login.c_str());
                }
            }, [](const std::string &error) {
                NSLog(@"error: %s", error.c_str());
            }));
}


@end
