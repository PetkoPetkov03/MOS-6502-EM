cmake_minimum_required(VERSION 3.10)

project(MOS_6502_EMULATOR, VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILE src/MOS.cpp)

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/dist")

add_executable(MOS ${SOURCE_FILE})

set_target_properties(MOS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

if(NOT EXISTS ${OUTPUT_DIR})
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
endif()

target_link_libraries(MOS stdc++)
