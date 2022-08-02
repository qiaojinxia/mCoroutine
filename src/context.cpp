//
// Created by cboy on 2022/8/1.
//

#include "context.h"

extern "C" {
    //rdi 是之前上下文的地址 rsi 是要恢复上下文的地址
    extern void switch_context(stack_co::Context *, stack_co::Context *) asm("switch_context");
}

namespace stack_co {
    //初始协程的返回地址 和 栈顶
    void Context::prepare(Context::Callback ret, Context::Word rdi) {
        Word sp = get_stack_pointer(); //获取当前函数栈顶指针
        fill_registers(sp, ret, rdi);
    }

    bool Context::test() {
        char current;
        ptrdiff_t diff = std::distance(std::begin(_stack), &current);
        return diff >= 0 && diff < STACK_SIZE;
    }

    Context::Word Context::get_stack_pointer() {
        auto sp = std::end(_stack) - sizeof(Word);
        sp = decltype(sp)(reinterpret_cast<size_t>(sp) & (~0xF));
        return sp;
    }

    void Context::fill_registers(Word sp, Callback ret, Word rdi, ...) {
        ::memset(_registers, 0, sizeof _registers);
        auto pRet = (Word *) sp;
        *pRet = (Word) ret;
        _registers[RSP] = sp; //栈顶
        _registers[RET] = *pRet; //返回地址
        _registers[RDI] = rdi; //previous 之前协程的context
    }

    void Context::switch_from(Context *previous) {
        switch_context(previous, this);
    }

}