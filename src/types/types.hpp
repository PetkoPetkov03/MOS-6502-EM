#ifndef MOS_TYPES
#define MOS_TYPES

#include "panic.h"
#include <array>
#include <stdint.h>
using Byte = uint8_t;
using Word = uint16_t;

using u32 = uint32_t;
using u8 = uint8_t;

template <typename T>
using Vec = std::array<T, 2>;

#endif // !MOS_TYPES
