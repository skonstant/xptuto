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

using namespace xptuto;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    auto x = Xptuto::make_instance();
    for (const auto &user : x->get_users()) {
        NSLog(@"Got user with name: %s", user.name.c_str());
    }


}


@end
