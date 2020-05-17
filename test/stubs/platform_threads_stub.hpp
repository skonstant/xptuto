//
// Created by stephane on 17/05/2020.
//

#ifndef XPTUTO_PLATFORMTHREADSSTUB_HPP
#define XPTUTO_PLATFORMTHREADSSTUB_HPP

#include "base_threads.hpp"
#include "thread_func.hpp"

class PlatformThreadsStub : public BaseThreads {
public:
    void run_on_main_thread(const std::shared_ptr<xptuto::ThreadFunc> &func) override;

    bool is_main_thread() override;
};


#endif //XPTUTO_PLATFORMTHREADSSTUB_HPP
