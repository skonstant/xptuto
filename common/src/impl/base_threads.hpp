//
// Created by Stephane Konstantaropoulos on 18/05/2020.
//

#ifndef XPTUTO_BASE_THREADS_HPP
#define XPTUTO_BASE_THREADS_HPP

#include "platform_threads.hpp"

class BaseThreads : public xptuto::PlatformThreads {

public:
    void create_thread(const std::string &name, const std::shared_ptr<xptuto::ThreadFunc> &func) override;
};


#endif //XPTUTO_BASE_THREADS_HPP
