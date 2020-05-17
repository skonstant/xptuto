//
// Created by stephane on 17/05/2020.
//

#include "web_threads.hpp"

using namespace xptuto;

void WebThreads::create_thread(const std::string &name, const std::shared_ptr<ThreadFunc> &func) {

}

void WebThreads::run_on_main_thread(const std::shared_ptr<ThreadFunc> &func) {

}

bool WebThreads::is_main_thread() {
    return false;
}
