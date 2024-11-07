#ifndef CYCLEINFO
#define CYCLEINFO

#include "../types/types.hpp"
#include "../memory/memory.hpp"
#include <unordered_map>

struct CPU;

struct CycleInfo {
  std::unordered_map<Byte, u8> I2CMap;
  std::unordered_map<Byte, u8> I2PCMap;

  std::unordered_map<Byte, void (*)(CPU &, u32 &, Mem &)> I2FuncMap;
  void Init(CPU &cpu);

  void AddFunc(Byte op, void func(CPU &, u32 &, Mem &)) {
    I2FuncMap[op] = func;
  }
};


u32 LoadCycles(Mem &memory, CycleInfo& ci, Word start_address);

#endif // !CYCLEINFO
