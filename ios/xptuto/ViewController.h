//
//  ViewController.h
//  xptuto
//
//  Created by Stephane Konstantaropoulos on 02/05/2020.
//  Copyright © 2020 example. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property IBOutlet UITextField *userInput;
@property IBOutlet UIActivityIndicatorView *progress;
@property IBOutlet UIButton *reposButton;
@property IBOutlet UIImageView *avatar;
@property IBOutlet UILabel *userNameLabel;
@property IBOutlet UILabel *createdLabel;
@property IBOutlet UILabel *notFoundLabel;

@property NSDateFormatter *dateFormatter;

@end

