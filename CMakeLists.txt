cmake_minimum_required(VERSION 3.10)

project(MOS_6502_EMULATOR, VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILE src/MOS.cpp)
set(CPU_FILE src/cpu/cpu.cpp)
set(MEMORY_FILE src/memory/memory.cpp)

add_library(cpu SHARED ${CPU_FILE} )
add_library(memory SHARED ${MEMORY_FILE})

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/dist")

add_executable(MOS ${SOURCE_FILE})

target_link_libraries(MOS PRIVATE memory)
target_link_libraries(MOS PRIVATE cpu)

set_target_properties(MOS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

if(NOT EXISTS ${OUTPUT_DIR})
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
endif()
