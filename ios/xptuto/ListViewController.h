//
// Created by Stephane Konstantaropoulos on 25/05/2020.
// Copyright (c) 2020 example. All rights reserved.
//

#import <UIKit/UIKit.h>

static NSString *const USERNAME = @"userName";

@interface ListViewController : UITableViewController <UITableViewDataSource, UITableViewDelegate>

@property NSString *userLogin;

@end