//
// Created by stephane on 17/05/2020.
//

#include "platform_threads_stub.hpp"
#include <thread>

using namespace xptuto;

void PlatformThreadsStub::create_thread(const std::string &name, const std::shared_ptr<ThreadFunc> &func) {
    std::thread t([](const std::shared_ptr<ThreadFunc> &f) {
        f->run();
    }, func);
    t.detach();
}

void PlatformThreadsStub::run_on_main_thread(const std::shared_ptr<ThreadFunc> &func) {
    func->run();
}

bool PlatformThreadsStub::is_main_thread() {
    return false;
}
