//
// Created by Stephane Konstantaropoulos on 18/05/2020.
//
#ifdef __APPLE__

#ifndef XPTUTO_APPLE_THREADS_HPP
#define XPTUTO_APPLE_THREADS_HPP

#include "base_threads.hpp"

class AppleThreads : public BaseThreads {
public:
    void run_on_main_thread(const std::shared_ptr<xptuto::ThreadFunc> &func) override;

    bool is_main_thread() override;
};


#endif //XPTUTO_APPLE_THREADS_HPP

#endif