//
// Created by Stephane Konstantaropoulos on 18/05/2020.
//
#ifdef __APPLE__

#import <Foundation/Foundation.h>

#include "apple_threads.hpp"
#include "thread_func.hpp"

using namespace xptuto;

void AppleThreads::run_on_main_thread(const std::shared_ptr<ThreadFunc> &func) {
    if (is_main_thread()) {
        func->run();
    } else {
        __block auto f = func;

        dispatch_async(dispatch_get_main_queue(), ^(void) {
            f->run();
        });
    }
}

bool AppleThreads::is_main_thread() {
    return NSThread.isMainThread;
}


#endif