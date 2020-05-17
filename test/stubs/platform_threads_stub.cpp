//
// Created by stephane on 17/05/2020.
//

#include "platform_threads_stub.hpp"

using namespace xptuto;

void PlatformThreadsStub::run_on_main_thread(const std::shared_ptr<ThreadFunc> &func) {
    func->run();
}

bool PlatformThreadsStub::is_main_thread() {
    return false;
}
