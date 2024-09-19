#include "../memory/memory.hpp"
#include "../types/types.hpp"
#include <unordered_map>

#ifndef MOS_CPU_DEF
#define MOS_CPU_DEF
struct CPU {
  Word PC; // program counter
  Byte SP; // stack pointer

  // registers
  Byte ACC;
  Byte RegisterX;
  Byte RegisterY;

  // status flags
  Byte C : 1; // carry
  Byte Z : 1; // zero
  Byte I : 1; // interupt
  Byte D : 1; // decimal
  Byte B : 1; // break;
  Byte O : 1; // overflow
  Byte N : 1; // negative

  void Reset(Mem &memory);

  Byte Fetch(u32 &Cycles, Mem &memory);

  Word FetchWord(u32 &Cycles, Mem &memory);

  Byte ReadByte(u32 &Cycles, Byte Address, Mem &memory);

  Word ReadWord(u32 &Cycles, Word Address, Mem &memory);

  void PushByte(Byte Value, u32 &Cycles, Mem &memory);

  Byte PullByte(u32 &Cycles, Mem &memory);

  static constexpr Byte INS_LDA_IM = 0xA9, INS_LDA_ZP = 0xA5,
                        INS_LDA_ZPX = 0xB5, INS_LDA_ABS = 0xAD,
                        INS_LDA_ABSX = 0xBD, INS_LDA_ABSY = 0xB9,
                        INS_LDA_INRX = 0xA1, INS_LDA_INRY = 0xB1,
                        INS_LDX_IM = 0xA2, INS_LDX_ZP = 0xA6,
                        INS_LDX_ZPY = 0xB6, INS_LDX_ABS = 0xAE,
                        INS_LDX_ABSY = 0xBE, INS_LDY_IM = 0xA0,
                        INS_LDY_ZP = 0xA4, INS_LDY_ZPX = 0xB4,
                        INS_LDY_ABS = 0xAC, INS_LDY_ABSX = 0xBC,
                        INS_JMP_ABS = 0x4C, INS_JMP_INR = 0x6C, INS_JSR = 0x20,
                        INS_STR_AZP = 0x85, INS_STR_AZPX = 0x95,
                        INS_STR_AABS = 0x8D, INS_STR_AABSX = 0x9D,
                        INS_STR_AABSY = 0x99, INS_STR_XZP = 0x86,
                        INS_STR_XZPY = 0x96, INS_STR_XABS = 0x8E,
                        INS_STR_YZP = 0x84, INS_STR_YZPX = 0x94,
                        INS_STR_YABS = 0x8C, INS_TAX = 0xAA, INS_TAY = 0xA8,
                        INS_TSX = 0xBA, INS_TXA = 0x8A, INS_TXS = 0x9A,
                        INS_TYA = 0x98, INS_PHA = 0x48, INS_PHP = 0x08,
                        INS_PLA = 0x68, INS_PLP = 0x28, INS_AND_IM = 0x29,
                        INS_AND_ZP = 0x25, INS_AND_ZPX = 0x35,
                        INS_AND_ABS = 0x2D, INS_AND_ABSX = 0x3D,
                        INS_AND_ABSY = 0x39, INS_AND_INRX = 0x21,
                        INS_AND_INRY = 0x31;

  void SetImmediate(u32 &Cycles, Byte &Register, Mem &memory);

  void SetZeroPage(u32 &Cycles, Byte &Register, Mem &memory);

  void SetZeroPageByRegister(u32 &Cycles, Byte &Register, Byte IncRegister,
                             Mem &memory);

  void SetAbsolute(u32 &Cycles, Byte &Register, Mem &memory);

  void SetAbsoluteByRegister(u32 &Cycles, Byte &Register, Byte IncRegister,
                             Mem &memory);

  // check zero page overflow
  void CheckZPOverflow(Byte Address);

  void SetStatusZN(Byte Register);

  void ZeroPageWrapAround(u32 &Cycles, Byte &Address);

  void LoadFlags(u32 &Cycles, Byte Flags);

  void Execute(u32 Cycles, Mem &memory);
};

u32 LoadCycles(Mem &memory, std::unordered_map<Byte, u8> I2CMap,
               std::unordered_map<Byte, u8> I2PCMap, Word start_address);
#endif // !MOS_CPU_DEF
