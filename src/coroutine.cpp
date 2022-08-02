//
// Created by cboy on 2022/8/1.
//
#include "coroutine.h"
namespace stack_co{
    //当上下文恢复后 调用实际运行函数
    void Coroutine::call_when_finish(Coroutine *coroutine) {
        auto &routine = coroutine->_entry;
        auto &runtime = coroutine->_runtime;
        if (routine) routine();
        runtime ^= (Status::EXIT | Status::RUNNING);
//        coroutine->yield();
        yield();//函数运行后让出调度
    }

    bool Coroutine::test() {
        return current()._context.test();
    }

    //让出当前协程
    void Coroutine::yield() {
        auto &coroutine = current();
        auto &currentContext = coroutine._context;

        coroutine._master->pop();

        auto &previousContext = current()._context;
        previousContext.switch_from(&currentContext);
    }

    Status Coroutine::resume() {
        if (!(_runtime ^ Status::EXIT)){
            return _runtime;
        }
        if (!(_runtime & Status::RUNNING)) {
            //第一次 启动协程保存上下文信息
            _context.prepare(Coroutine::call_when_finish, this);
            _runtime |= Status::RUNNING;
            _runtime &= ~Status::EXIT;
        }
        //取一个协程 用来保存当前协程上下文
        auto previous = _master->current();
        //把当前协程 推入协程栈中
        _master->push(shared_from_this());
        //将当前cpu寄存器信息保存到 previous Coroutine的上下文中
        _context.switch_from(&previous->_context);
        return _runtime;
    }

    //获取当前协程状态
    Status Coroutine::runtime() const {
        return _runtime;
    }

    //获取当前环境运行的协程
    Coroutine &Coroutine::current() {
        return *Environment::instance().current();
    }

}