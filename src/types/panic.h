#include <iostream>
#include <ostream>
#ifndef MOSPANIC
#define PANIC(message)                           \
    do {                                         \
        std::cerr << "Panic: " << message << std::endl; \
        std::exit(EXIT_FAILURE);                 \
    } while (0)
#endif // !MOSPANIC

