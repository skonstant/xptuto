//
// Created by stephane on 17/05/2020.
//
#ifdef EMSCRIPTEN

#ifndef XPTUTO_WEBTHREADS_HPP
#define XPTUTO_WEBTHREADS_HPP

#include "platform_threads.hpp"
#include "thread_func.hpp"
#include <queue>
#include <thread>
#include <optional>

class WebThreads : public xptuto::PlatformThreads {
public:

    WebThreads();

    void create_thread(const std::string &name, const std::shared_ptr<xptuto::ThreadFunc> &func) override;

    void run_on_main_thread(const std::shared_ptr<xptuto::ThreadFunc> &func) override;

    bool is_main_thread() override;

    static inline std::queue<std::shared_ptr<xptuto::ThreadFunc>> functionsToRun;
    static inline std::optional<std::thread::id> main_thread_id;
};


#endif //XPTUTO_WEBTHREADS_HPP

#endif
