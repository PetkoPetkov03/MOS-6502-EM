cmake_minimum_required(VERSION 3.10)

project(MOS_6502_EMULATOR, VERSION 1.0)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FetchContent_Declare(
  fltk
  GIT_REPOSITORY https://github.com/fltk/fltk.git
  GIT_TAG release-1.3.9
)


# Set options to avoid building examples or tests of FLTK
set(FLTK_BUILD_TEST OFF CACHE BOOL "Build FLTK test programs")
set(FLTK_BUILD_EXAMPLES OFF CACHE BOOL "Build FLTK example programs")
set(FLTK_BUILD_FLUID OFF CACHE BOOL "Build FLUID (the FLTK UI designer)")
FetchContent_MakeAvailable(fltk)



set(SOURCE_FILE src/MOS.cpp)
set(CPU_FILE src/cpu/cpu.cpp)
set(MEMORY_FILE src/memory/memory.cpp)
set(INTERFACE_FILE src/interface/interface.cpp)

add_library(cpu SHARED ${CPU_FILE} )
add_library(memory SHARED ${MEMORY_FILE})
add_library(interface SHARED ${INTERFACE_FILE})

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/dist")

add_executable(MOS ${SOURCE_FILE})

target_include_directories(MOS PRIVATE ${fltk_SOURCE_DIR} ${fltk_BINARY_DIR})
target_link_libraries(MOS PRIVATE memory cpu memory interface)
target_link_libraries(MOS PRIVATE fltk fltk_gl)

set_target_properties(MOS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

if(NOT EXISTS ${OUTPUT_DIR})
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
endif()
