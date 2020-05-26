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
#include "apple_threads.hpp"
#include "xptuto_impl.hpp"
#import "DJIMarshal+Private.h"
#import "ListViewController.h"
#import <SDWebImage/SDWebImage.h>

using namespace xptuto;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    _dateFormatter = [NSDateFormatter new];
    _dateFormatter.dateStyle = NSDateFormatterMediumStyle;
    _dateFormatter.timeStyle = NSDateFormatterNoStyle;
}

- (void)hideDetails {
    _userNameLabel.hidden = true;
    _createdLabel.hidden = true;
    _avatar.image = nullptr;
    _avatar.hidden = true;
    _reposButton.hidden = true;
    _notFoundLabel.hidden = true;
}

- (void)showDetails:(const User &)user {
    _userNameLabel.text = djinni::String::fromCpp(user.login);
    [_avatar sd_setImageWithURL:[NSURL URLWithString:djinni::String::fromCpp(user.avatar_url)]];
    _createdLabel.text = [_dateFormatter stringFromDate:djinni::Date::fromCpp(user.created_at)];

    _userNameLabel.hidden = false;
    _createdLabel.hidden = false;
    _avatar.hidden = false;
    _reposButton.hidden = false;
}

- (IBAction) textFieldDidEndEditing:(UITextField *)textField {
    if (_userInput.text.length) {
        [_progress startAnimating];
        [self hideDetails];

        __weak auto welf = self;

        auto x = Xptuto::get_instance();

        if (!x) {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
            NSString *cacheDirectory = [paths objectAtIndex:0];

            x = Xptuto::make_instance(std::make_shared<AppleHttpClient>(), std::make_shared<AppleThreads>(),
                    djinni::String::toCpp(cacheDirectory));
        }

        x->get_user(djinni::String::toCpp(_userInput.text), std::make_shared<GetUserCbImpl>(
                [welf](const User &user) {
                    [welf.progress stopAnimating];
                    NSLog(@"Got user with login: %s", user.login.c_str());
                    [welf showDetails:user];
                }, [welf](const std::string &error) {
                    NSLog(@"error: %s", error.c_str());
                    [welf.progress stopAnimating];
                    welf.notFoundLabel.hidden = false;
                }));
    }
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([segue.identifier isEqualToString:@"repos"]) {
        auto listVC = (ListViewController *) segue.destinationViewController;
        listVC.userLogin = _userNameLabel.text;
    }
    [super prepareForSegue:segue sender:sender];
}


- (void)encodeRestorableStateWithCoder:(NSCoder *)coder {
    [super encodeRestorableStateWithCoder:coder];
    if (self.userNameLabel.text.length) {
        [coder encodeObject:_userNameLabel.text forKey:USERNAME];
    }
}

- (void)decodeRestorableStateWithCoder:(NSCoder *)coder {
    [super decodeRestorableStateWithCoder:coder];
    if ([coder containsValueForKey:USERNAME]) {
        _userInput.text = [coder decodeObjectForKey:USERNAME];
        [self textFieldDidEndEditing:_userInput];
    }
}



@end
