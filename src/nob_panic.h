#ifndef __NOB_PANIC__
#include "stdio.h"
#include <stdlib.h>
#define PANIC(message) \
  do {                  \
    perror(message);    \
    exit(EXIT_FAILURE); \
  } while(0);            
#endif
