
cmake_minimum_required(VERSION 3.10)

project(MOS_6502_EMULATOR VERSION 1.0)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Fetch FLTK library
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

# Source files
set(SOURCE_FILE src/MOS.cpp)

set(MEMORY_FILE src/memory/memory.cpp)

set(CPU_FILE src/cpu/cpu.cpp)
set(INTERFACE_FILE src/interface/interface.cpp)

# Libraries

add_library(memory SHARED ${MEMORY_FILE})
add_library(cpu SHARED ${CPU_FILE})
add_library(interface SHARED ${INTERFACE_FILE})

# Executable
add_executable(MOS ${SOURCE_FILE})

# Include directories

target_include_directories(MOS PRIVATE ${fltk_SOURCE_DIR} ${fltk_BINARY_DIR})
target_include_directories(interface PRIVATE ${fltk_SOURCE_DIR} ${fltk_BINARY_DIR})


if(MINGW)
    message(STATUS "Building for Windows (MinGW)...")

    # Link CPU and OPIMPL to MEMORY
    target_link_libraries(cpu PRIVATE memory)

    # Link interface with FLTK and system libraries for MinGW
    target_link_libraries(interface PRIVATE fltk fltk_gl cpu memory)

    # Add required system libraries for MinGW (Windows)
    target_link_libraries(interface PRIVATE opengl32 glu32 gdi32)

    # Link MOS executable
    target_link_libraries(MOS PRIVATE memory cpu interface)
else()
  target_link_libraries(MOS PRIVATE memory cpu interface fltk fltk_gl )
endif()
# Output directory
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/dist")

# Ensure output directory exists
if(NOT EXISTS ${OUTPUT_DIR})
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
endif()

# Set target properties after defining the target
set_target_properties(MOS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Optionally handle build for Windows and Linux
if(MINGW)
    message(STATUS "Building for Windows (MinGW)...")
else()
    message(STATUS "Building for Linux...")
endif()

