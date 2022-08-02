//
// Created by cboy on 2022/8/1.
//

#ifndef MCOROUTINE_COROUTINE_H
#define MCOROUTINE_COROUTINE_H
#include "status.h"
#include "context.h"
#include "environment.h"


#include <functional>
#include <memory>
namespace stack_co {
    class Environment;

    class Coroutine : public std::enable_shared_from_this<Coroutine> {
        friend class Environment;

        friend class Context;

    public:
        static Coroutine &current();

        Status runtime() const;

        // 核心操作：resume和yield

        // usage: Coroutine::current().yield()
        static void yield();


        // Note1: 允许处于EXIT状态的协程重入，从而再次resume
        //        如果不使用这种特性，则用exit() / running()判断
        //
        // Note2: 返回值可以得知resume并执行后的运行时状态
        //        但是这个值只适用于简单的场合
        //        如果接下来其它协程的运行也影响了该协程的状态
        //        那建议用runtime()获取
        Status resume();

        static bool test();

        Coroutine(const Coroutine &) = delete;

        Coroutine(Coroutine &&) = delete;

        Coroutine &operator=(const Coroutine &) = delete;

        Coroutine &operator=(Coroutine &&) = delete;


    public:
        // 构造Coroutine执行函数，entry为函数入口，对应传参为arguments...
        // Note: 出于可重入的考虑，entry强制为值语义
        template<typename Entry, typename ...Args>
        Coroutine(Environment *master, Entry entry, Args ...arguments)
                : _entry([=] { entry(/*std::move*/(arguments)...); }),
                  _master(master) {}
    private:
        static void call_when_finish(Coroutine *coroutine);
    private:
        Status _runtime{};

        Context _context{};

        std::function<void()> _entry;

        Environment *_master;
    };

}
#endif //MCOROUTINE_COROUTINE_H
