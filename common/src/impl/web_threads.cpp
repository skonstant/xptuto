//
// Created by stephane on 17/05/2020.
//
#ifdef EMSCRIPTEN

#include "web_threads.hpp"
#include <thread>

#include <emscripten/html5.h>

using namespace xptuto;

EM_BOOL main_loop(double time, void* userData) {

    if (!WebThreads::main_thread_id) {
        WebThreads::main_thread_id = std::this_thread::get_id();
    }

    if (WebThreads::functionsToRun.empty()) {
        return EM_TRUE;
    }

    auto func = WebThreads::functionsToRun.front();
    func->run();

    const std::lock_guard<std::mutex> lock(WebThreads::functionsMutex);
    WebThreads::functionsToRun.pop();
    // Return true to keep the loop running.
    return EM_TRUE;
}

WebThreads::WebThreads() {
    emscripten_request_animation_frame_loop(main_loop, nullptr);
}

void WebThreads::run_on_main_thread(const std::shared_ptr<ThreadFunc> &func) {
    const std::lock_guard<std::mutex> lock(functionsMutex);
    functionsToRun.push(func);
}

bool WebThreads::is_main_thread() {
    return std::this_thread::get_id() == main_thread_id;
}

#endif
