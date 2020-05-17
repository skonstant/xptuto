//
// Created by Stephane Konstantaropoulos on 18/05/2020.
//

#include "base_threads.hpp"

#include <thread>
#include "thread_func.hpp"

using namespace xptuto;

void BaseThreads::create_thread(const std::string &name, const std::shared_ptr<ThreadFunc> &func) {
    std::thread t([](const std::shared_ptr<ThreadFunc> &f) {
        f->run();
    }, func);
    t.detach();
}
