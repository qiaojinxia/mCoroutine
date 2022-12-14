//
// Created by cboy on 2022/8/1.
//

#ifndef MYCOROUTINE_CONTEXT_H
#define MYCOROUTINE_CONTEXT_H
#include <cstddef>
#include <cstring>
#include <iterator>
namespace stack_co{
    class Coroutine;
    /**
   * The context of coroutine(in x86-64)
   *
   * low | _registers[0]: r15  |
   *     | _registers[1]: r14  |
   *     | _registers[2]: r13  |
   *     | _registers[3]: r12  |
   *     | _registers[4]: r9   |
   *     | _registers[5]: r8   |
   *     | _registers[6]: rbp  |
   *     | _registers[7]: rdi  |
   *     | _registers[8]: rsi  |
   *     | _registers[9]: ret  |
   *     | _registers[10]: rdx |
   *     | _registers[11]: rcx |
   *     | _registers[12]: rbx |
   * hig | _registers[13]: rsp |
   *
   */
    class Context final {
    public:
        using Callback = void (*)(Coroutine *);
        using Word = void *;

        constexpr static size_t STACK_SIZE = 1 << 17;
        constexpr static size_t RDI = 7;
        constexpr static size_t RSI = 8;
        constexpr static size_t RET = 9;
        constexpr static size_t RSP = 13;

    public:
        void prepare(Callback ret, Word rdi);

        void switch_from(Context *previous);

        bool test();

    private:
        Word get_stack_pointer();

        void fill_registers(Word sp, Callback ret, Word rdi, ...);

    private:
        /**
         * We must ensure that registers are at the top of the memory layout.
         *
         * So the Context must have no virtual method, and len at least 14!
         */
        Word _registers[14];

        char _stack[STACK_SIZE];
    };
}

#endif //MYCOROUTINE_CONTEXT_H
