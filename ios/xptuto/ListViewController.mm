//
// Created by Stephane Konstantaropoulos on 25/05/2020.
// Copyright (c) 2020 example. All rights reserved.
//

#import "ListViewController.h"
#import "DJIMarshal+Private.h"

#include "get_repos_cb_impl.hpp"
#include "apple_http_client.hpp"
#include "apple_threads.hpp"
#include "xptuto_impl.hpp"

using namespace xptuto;

@implementation ListViewController {
    std::vector<Repo> repos;
    NSDateFormatter *dateFormatter;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    self.title = [NSString stringWithFormat:@"Repositories for %@", self.userLogin];

    dateFormatter = [NSDateFormatter new];
    dateFormatter.dateStyle = NSDateFormatterMediumStyle;
    dateFormatter.timeStyle = NSDateFormatterNoStyle;

    __weak auto welf = self;

    auto x = Xptuto::make_instance(std::make_shared<AppleHttpClient>(), std::make_shared<AppleThreads>());
    x->get_repos_for_user_name(djinni::String::toCpp(_userLogin), std::make_shared<GetReposCbImpl>(
            [welf](const std::vector<Repo> &r, const User &user) {
                auto strongSelf = welf;
                if (strongSelf) {
                    strongSelf->repos = r;
                }
                [welf.tableView reloadData];
            }, [welf](const std::string &error) {

            }));
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return repos.size();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"repoCell"];

    Repo repo = repos.at(indexPath.row);
    cell.textLabel.text = djinni::String::fromCpp(repo.full_name);
    cell.detailTextLabel.text = [dateFormatter stringFromDate:djinni::Date::fromCpp(repo.created_at)];
    return cell;

}


@end