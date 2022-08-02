//
// Created by cboy on 2022/8/1.
//
#ifndef MCOROUTINE_ENVIROMENT_H
#define MCOROUTINE_ENVIROMENT_H
#include "coroutine.h"
#include "array"

#include <cstddef>
#include <cstring>
#include <functional>
#include <memory>
namespace stack_co {
    class Environment{
        friend class Coroutine;
    public:
        // Thread-local instance
        static Environment &instance();
        // No copy constructor
        Environment(const Environment&) = delete;
        // No Assignment Operator
        Environment &operator=(const Environment &) = delete;
        // Factory method
        template<typename Entry, typename ...Args>
        inline std::shared_ptr<Coroutine> create_coroutine(Entry &&entry, Args &&...arguments){
            return std::make_shared<Coroutine>(
                    this, std::forward<Entry>(entry), std::forward<Args>(arguments)...);
        }
        // Get current coroutine in the stack
        Coroutine *current();
    private:
        // No explicit constructor
        Environment();

        void push(std::shared_ptr<Coroutine> coroutine);

        void pop();
    private:
        // Coroutine calling stack
        std::array<std::shared_ptr<Coroutine>, 1024> _c_stack;

        // Top of the coroutine calling stack
        size_t _c_stack_top;

        // Main coroutine(root)
        std::shared_ptr<Coroutine> _main;
    };


}
#endif //MCOROUTINE_ENVIROMENT_H
