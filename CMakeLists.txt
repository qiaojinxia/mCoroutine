cmake_minimum_required(VERSION 3.19)
project(mCoroutine )
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS asm)
ENABLE_LANGUAGE(ASM)
SET(CMAKE_ASM_NASM_FLAGS "-g")

set(CMAKE_CXX_STANDARD 17)
set(ASM)
add_executable(mCoroutine src/main.cpp src/environment.h src/environment.cpp src/coroutine.h src/coroutine.cpp src/context.h src/context.cpp src/switch_context.asm)
