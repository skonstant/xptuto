//
// Created by stephane on 17/05/2020.
//

#ifndef XPTUTO_THREAD_FUNC_IMPL_HPP
#define XPTUTO_THREAD_FUNC_IMPL_HPP

#include "thread_func.hpp"
#include <functional>

class ThreadFuncImpl : public xptuto::ThreadFunc {
public:
    explicit ThreadFuncImpl(std::function<void()> func) : m_func(std::move(func)) {}

    void run() override {
        m_func();
    }

private:
    const std::function<void()> m_func;
};


#endif //XPTUTO_THREAD_FUNC_IMPL_HPP
