//
// Created by stephane on 17/05/2020.
//

#ifndef XPTUTO_WEBTHREADS_HPP
#define XPTUTO_WEBTHREADS_HPP

#include "platform_threads.hpp"

class WebThreads : public xptuto::PlatformThreads {
public:
    void create_thread(const std::string &name, const std::shared_ptr<ThreadFunc> &func) override;

    void run_on_main_thread(const std::shared_ptr<ThreadFunc> &func) override;

    bool is_main_thread() override;
};


#endif //XPTUTO_WEBTHREADS_HPP
