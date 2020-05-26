//
//  AppDelegate.m
//  xptuto
//
//  Created by Stephane Konstantaropoulos on 02/05/2020.
//  Copyright © 2020 example. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    return true;
}

- (BOOL)application:(UIApplication *)application shouldSaveApplicationState:(NSCoder *)coder {
    return true;
}

- (BOOL)application:(UIApplication *)application shouldRestoreApplicationState:(NSCoder *)coder {
    return true;
}

- (BOOL)application:(UIApplication *)application shouldSaveSecureApplicationState:(NSCoder *)coder {
    return true;
}

- (BOOL)application:(UIApplication *)application shouldRestoreSecureApplicationState:(NSCoder *)coder {
    return true;
}


@end
