#include <iostream>
#include "environment.h"
#include <unistd.h>
namespace stack_co {

    namespace this_coroutine {

        inline void yield() {
            return ::stack_co::Coroutine::yield();
        }

    } // namespace this_coroutine

    inline bool test() {
        return Coroutine::test();
    }

    inline Environment &open() {
        return Environment::instance();
    }


} // namespace stack_co

void where() {
    std::cout << "running code in a "
              << (stack_co::test() ? "coroutine" : "thread")
              << std::endl;
}

void print1() {
    std::cout << 1 << std::endl;
    stack_co::this_coroutine::yield();
    std::cout << 2 << std::endl;
}

void print2(int i, stack_co::Coroutine *co1) {
    std::cout << i << std::endl;
    co1->resume();
    where();
    std::cout << "bye" << std::endl;
}

int main() {
    auto &env = stack_co::open();
    auto co1 = env.create_coroutine(print1);
    auto co2 = env.create_coroutine(print2, 3, co1.get());
    while(true){
        co1->resume();
        sleep(1);
        co2->resume();
    }
    where();
    return 0;
}
