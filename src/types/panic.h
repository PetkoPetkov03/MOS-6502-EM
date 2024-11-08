#include <iostream>
#include <ostream>
#ifndef __MOSPANIC__
#define __MOSPANIC__

#define PANIC(message)                           \
    do {                                         \
        std::cerr << "\033[1;31m Panic: " << message << "\033[0m" << std::endl; \
        std::exit(EXIT_FAILURE);                 \
    } while (0)
#endif // !MOSPANIC

