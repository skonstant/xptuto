//
// Created by stephane on 17/05/2020.
//
#ifdef EMSCRIPTEN

#include "web_threads.hpp"
#include <thread>

#include <emscripten/html5.h>

using namespace xptuto;

EM_BOOL one_iter(double time, void* userData) {
    if (WebThreads::functionsToRun.empty()) {
        return EM_FALSE;
    }

    auto func = WebThreads::functionsToRun.front();
    func->run();

    WebThreads::functionsToRun.pop();

    if (WebThreads::functionsToRun.empty()) {
        return EM_FALSE;
    } else {
        // Return true to keep the loop running.
        return EM_TRUE;
    }
}

EM_BOOL get_main_thread(double time, void* userData) {
    WebThreads::main_thread_id = std::this_thread::get_id();
    return EM_FALSE;
}

WebThreads::WebThreads() {
    emscripten_request_animation_frame_loop(get_main_thread, nullptr);
}

void WebThreads::run_on_main_thread(const std::shared_ptr<ThreadFunc> &func) {
    functionsToRun.push(func);
    emscripten_request_animation_frame_loop(one_iter, nullptr);
}

bool WebThreads::is_main_thread() {
    return std::this_thread::get_id() == main_thread_id;
}

#endif
