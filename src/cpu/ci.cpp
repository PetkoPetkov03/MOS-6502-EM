#include "ci.hpp"

void CycleInfo::Init(CPU &cpu) {
  I2CMap = {
      {CPU::INS_LDA_IM, 2},    {CPU::INS_LDA_ZP, 3},    {CPU::INS_LDA_ZPX, 4},
      {CPU::INS_LDA_ABS, 4},   {CPU::INS_LDA_ABSX, 4},  {CPU::INS_LDA_ABSY, 4},
      {CPU::INS_LDA_INRX, 6},  {CPU::INS_LDA_INRY, 5},  {CPU::INS_LDX_IM, 2},
      {CPU::INS_LDX_ZP, 3},    {CPU::INS_LDX_ZPY, 4},   {CPU::INS_LDX_ABS, 4},
      {CPU::INS_LDX_ABSY, 4},  {CPU::INS_LDY_IM, 2},    {CPU::INS_LDY_ZP, 3},
      {CPU::INS_LDY_ZPX, 4},   {CPU::INS_LDY_ABS, 4},   {CPU::INS_LDY_ABSX, 4},
      {CPU::INS_STR_AZP, 3},   {CPU::INS_STR_AZPX, 4},  {CPU::INS_STR_AABS, 4},
      {CPU::INS_STR_AABSX, 5}, {CPU::INS_STR_AABSY, 5}, {CPU::INS_STR_XZP, 3},
      {CPU::INS_STR_XZPY, 4},  {CPU::INS_STR_XABS, 4},  {CPU::INS_STR_YZP, 3},
      {CPU::INS_STR_YZPX, 4},  {CPU::INS_STR_YABS, 4},  {CPU::INS_TAX, 2},
      {CPU::INS_TAY, 2},       {CPU::INS_TSX, 2},       {CPU::INS_TXA, 2},
      {CPU::INS_TXS, 2},       {CPU::INS_TYA, 2},       {CPU::INS_JSR, 6},
      {CPU::INS_JMP_ABS, 3},   {CPU::INS_JMP_INR, 5},   {CPU::INS_PHA, 3},
      {CPU::INS_PHP, 3},       {CPU::INS_PLA, 4},       {CPU::INS_PLP, 4},
      {CPU::INS_AND_IM, 2},    {CPU::INS_AND_ZP, 3},    {CPU::INS_AND_ZPX, 4},
      {CPU::INS_AND_ABS, 4},   {CPU::INS_AND_ABSX, 4},  {CPU::INS_AND_ABSY, 4},
      {CPU::INS_AND_INRX, 6},  {CPU::INS_AND_INRY, 5},  {CPU::INS_EOR_IM, 2},
      {CPU::INS_EOR_ZP, 3},    {CPU::INS_EOR_ZPX, 4},   {CPU::INS_EOR_ABS, 4},
      {CPU::INS_EOR_ABSX, 4},  {CPU::INS_EOR_ABSY, 4},  {CPU::INS_EOR_INRX, 6},
      {CPU::INS_EOR_INRY, 5},  {CPU::INS_NOP, 2},       {CPU::INS_ORA_IM, 2},
      {CPU::INS_ORA_ZP, 3},    {CPU::INS_ORA_ZPX, 4},   {CPU::INS_ORA_ABS, 4},
      {CPU::INS_ORA_ABSX, 4},  {CPU::INS_ORA_ABSY, 4},  {CPU::INS_ORA_INRX, 6},
      {CPU::INS_ORA_INRY, 5},
  };

  I2PCMap = {
      {CPU::INS_LDA_IM, 2},    {CPU::INS_LDA_ZP, 2},    {CPU::INS_LDA_ZPX, 2},
      {CPU::INS_LDA_ABS, 3},   {CPU::INS_LDA_ABSX, 3},  {CPU::INS_LDA_ABSY, 3},
      {CPU::INS_LDA_INRX, 2},  {CPU::INS_LDA_INRY, 2},  {CPU::INS_LDX_IM, 2},
      {CPU::INS_LDX_ZP, 2},    {CPU::INS_LDX_ZPY, 2},   {CPU::INS_LDX_ABS, 3},
      {CPU::INS_LDX_ABSY, 3},  {CPU::INS_LDY_IM, 2},    {CPU::INS_LDY_ZP, 2},
      {CPU::INS_LDY_ZPX, 2},   {CPU::INS_LDY_ABS, 3},   {CPU::INS_LDY_ABSX, 3},
      {CPU::INS_STR_AZP, 2},   {CPU::INS_STR_AZPX, 2},  {CPU::INS_STR_AABS, 3},
      {CPU::INS_STR_AABSX, 3}, {CPU::INS_STR_AABSY, 3}, {CPU::INS_STR_XZP, 2},
      {CPU::INS_STR_XZPY, 2},  {CPU::INS_STR_XABS, 3},  {CPU::INS_STR_YZP, 2},
      {CPU::INS_STR_YZPX, 2},  {CPU::INS_STR_YABS, 3},  {CPU::INS_TAX, 1},
      {CPU::INS_TAY, 1},       {CPU::INS_TSX, 1},       {CPU::INS_TXA, 1},
      {CPU::INS_TXS, 1},       {CPU::INS_TYA, 1},       {CPU::INS_JSR, 3},
      {CPU::INS_JMP_ABS, 3},   {CPU::INS_JMP_INR, 3},   {CPU::INS_PHA, 1},
      {CPU::INS_PHP, 1},       {CPU::INS_PLA, 1},       {CPU::INS_PLP, 1},
      {CPU::INS_AND_IM, 2},    {CPU::INS_AND_ZP, 2},    {CPU::INS_AND_ZPX, 2},
      {CPU::INS_AND_ABS, 3},   {CPU::INS_AND_ABSX, 3},  {CPU::INS_AND_ABSY, 3},
      {CPU::INS_AND_INRX, 2},  {CPU::INS_AND_INRY, 2},  {CPU::INS_EOR_IM, 2},
      {CPU::INS_EOR_ZP, 2},    {CPU::INS_EOR_ZPX, 2},   {CPU::INS_EOR_ABS, 3},
      {CPU::INS_EOR_ABSX, 3},  {CPU::INS_EOR_ABSY, 3},  {CPU::INS_EOR_INRX, 2},
      {CPU::INS_EOR_INRY, 2},  {CPU::INS_NOP, 1},       {CPU::INS_ORA_IM, 2},
      {CPU::INS_ORA_ZP, 2},    {CPU::INS_ORA_ZPX, 2},   {CPU::INS_ORA_ABS, 3},
      {CPU::INS_ORA_ABSX, 3},  {CPU::INS_ORA_ABSY, 3},  {CPU::INS_ORA_INRX, 2},
      {CPU::INS_ORA_INRY, 2},
  };

  AddFunc(CPU::INS_LDA_IM, LDA_IM);
  AddFunc(CPU::INS_LDA_ZP, LDA_ZP);
  AddFunc(CPU::INS_LDA_ZPX, LDA_ZPX);
  AddFunc(CPU::INS_LDA_ABS, LDA_ABS);
  AddFunc(CPU::INS_LDA_ABSX, LDA_ABSX);
  AddFunc(CPU::INS_LDA_ABSY, LDA_ABSY);
  AddFunc(CPU::INS_LDA_INRX, LDA_INRX);
  AddFunc(CPU::INS_LDA_INRY, LDA_INRY);
  AddFunc(CPU::INS_LDX_IM, LDX_IM);
  AddFunc(CPU::INS_LDX_ZP, LDX_ZP);
  AddFunc(CPU::INS_LDX_ZPY, LDX_ZPY);
  AddFunc(CPU::INS_LDX_ABS, LDX_ABS);
  AddFunc(CPU::INS_LDX_ABSY, LDX_ABSY);
  AddFunc(CPU::INS_LDY_IM, LDY_IM);
  AddFunc(CPU::INS_LDY_ZP, LDY_ZP);
  AddFunc(CPU::INS_LDY_ABS, LDY_ABS);
  AddFunc(CPU::INS_PHA, PHA);
  AddFunc(CPU::INS_PHP, PHP);
  AddFunc(CPU::INS_PLA, PLA);
  AddFunc(CPU::INS_PLP, PLP);
  AddFunc(CPU::INS_JSR, JSR);
  AddFunc(CPU::INS_STR_AZP, STRAZP);
  AddFunc(CPU::INS_STR_AZPX, STRAZPX);
  AddFunc(CPU::INS_STR_AABS, STRAABS);
  AddFunc(CPU::INS_STR_AABSX, STRAABSX);
  AddFunc(CPU::INS_STR_AABSY, STRAABSY);
  AddFunc(CPU::INS_STR_AINRX, STRAINRX);
  AddFunc(CPU::INS_STR_AINRX, STRAINRX);
  AddFunc(CPU::INS_STR_AINRY, STRAINRY);
  AddFunc(CPU::INS_STR_AZP, STRAZP);
  AddFunc(CPU::INS_STR_AZP, STRAZP);
  AddFunc(CPU::INS_JMP_ABS, JMP_ABS);
  AddFunc(CPU::INS_JMP_INR, JMP_INR);
  AddFunc(CPU::INS_TAX, TAX);
  AddFunc(CPU::INS_TAY, TAY);
  AddFunc(CPU::INS_TSX, TSX);
  AddFunc(CPU::INS_TXA, TXA);
  AddFunc(CPU::INS_TXS, TXS);
  AddFunc(CPU::INS_TYA, TYA);
  AddFunc(CPU::INS_AND_IM, AND_IM);
  AddFunc(CPU::INS_AND_ZP, AND_ZP);
  AddFunc(CPU::INS_AND_ZPX, AND_ZPX);
  AddFunc(CPU::INS_AND_ABS, AND_ABS);
  AddFunc(CPU::INS_AND_ABSX, AND_ABSX);
  AddFunc(CPU::INS_AND_ABSY, AND_ABSY);
  AddFunc(CPU::INS_AND_INRX, AND_INRX);
  AddFunc(CPU::INS_AND_INRY, AND_INRY);
  AddFunc(CPU::INS_EOR_IM, EOR_IM);
  AddFunc(CPU::INS_EOR_ZP, EOR_ZP);
  AddFunc(CPU::INS_EOR_ZPX, EOR_ZPX);
  AddFunc(CPU::INS_EOR_ABS, EOR_ABS);
  AddFunc(CPU::INS_EOR_ABSX, EOR_ABSX);
  AddFunc(CPU::INS_EOR_ABSY, EOR_ABSY);
  AddFunc(CPU::INS_EOR_INRX, EOR_INRX);
  AddFunc(CPU::INS_EOR_INRY, ORA_INRY);
  AddFunc(CPU::INS_ORA_IM, ORA_IM);
  AddFunc(CPU::INS_ORA_ZP, ORA_ZP);
  AddFunc(CPU::INS_ORA_ZPX, ORA_ZPX);
  AddFunc(CPU::INS_ORA_ABS, ORA_ABS);
  AddFunc(CPU::INS_ORA_ABSX, ORA_ABSX);
  AddFunc(CPU::INS_ORA_ABSY, ORA_ABSY);
  AddFunc(CPU::INS_ORA_INRX, ORA_INRX);
  AddFunc(CPU::INS_ORA_INRY, ORA_INRY);
  AddFunc(CPU::INS_CMP_IM, CMP_IM);
  AddFunc(CPU::INS_CMP_ZP, CMP_ZP);
  AddFunc(CPU::INS_CMP_ZPX, CMP_ZPX);
  AddFunc(CPU::INS_CMP_ABS, CMP_ABS);
  AddFunc(CPU::INS_CMP_ABSX, CMP_ABSX);
  AddFunc(CPU::INS_CMP_ABSY, CMP_ABSY);
  AddFunc(CPU::INS_CMP_INRX, CMP_INRX);
  AddFunc(CPU::INS_CMP_INRY, CMP_INRY);
}

u32 LoadCycles(Mem &memory, CycleInfo ci, Word start_address) {
  u32 total_cycles = 0;
  Word pc = start_address;

  while (true) {
    Byte opcode = memory[pc];
    if (ci.I2CMap.find(opcode) != ci.I2CMap.end()) {
      total_cycles += ci.I2CMap[opcode];

      switch (opcode) {
      case CPU::INS_JSR: {
        Word return_address = pc + ci.I2PCMap[opcode];

        pc = memory[pc + 1] | (memory[pc + 2] << 8);
        continue;
      } break;

      case CPU::INS_JMP_ABS: {
        pc = memory[pc + 1] | (memory[pc + 2] << 8);
        continue;
      } break;

      case CPU::INS_JMP_INR: {
        Word pointer = memory[pc + 1] | (memory[pc + 2] << 8);

        pc = memory[pointer] | (memory[pointer + 1] << 8);
        continue;
      } break;

      default: {
        pc += ci.I2PCMap[opcode];
      } break;
      }
    } else {
      printf("LoadCycles compleate!\n");
      break;
    }
  }

  return total_cycles;
}
