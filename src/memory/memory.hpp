#include "../types/types.hpp"
#ifndef __MOS_MEM_DEF__
#define __MOS_MEM_DEF__
#include <assert.h>

struct Mem {
  static constexpr u32 MAX_MEM = 1024 * 64;
  Byte Data[MAX_MEM];

  void Init();

  // read 1 byte
  Byte operator[](u32 Address) const;

  // write 1 byte
  Byte &operator[](u32 Address);

  // Write 2 bytes
  void WriteWord(u32 &Cycles, Word word, u32 Address);
};

#endif // !MOS_MEM_DEF
