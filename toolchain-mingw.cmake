
# mingw-toolchain.cmake

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross-compilers
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Set the resource compiler for MinGW
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Optionally, set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

