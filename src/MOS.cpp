#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "./cpu/cpu.hpp"
#include "./memory/memory.hpp"
#include "types/types.hpp"
#include "./interface/interface.hpp"

// http://www.6502.org/users/obelisk/index.html

int main() {
  Mem mem;
  CPU cpu;
  cpu.Reset(mem);

  Interface GUInterface;

  GUInterface.InitWindow();

  CycleInfo ci;

  ci.Init(cpu);

  // JSR
  /*mem[0xFFFC] = CPU::INS_JSR;*/
  /*mem[0xFFFD] = 0x42;*/
  /*mem[0xFFFE] = 0x42;*/
  /*mem[0x4242] = CPU::INS_LDA_IM;*/
  /*mem[0x4243] = 0x84;*/

  // LDX ZP
  /*mem[0xFFFC] = CPU::INS_LDX_ZP;*/
  /*mem[0xFFFD] = 0x43;*/
  /*mem[0x0043] = 0xFF;*/

  /*mem[0xFFFC] = CPU::INS_JSR;*/
  /*mem[0xFFFD] = 0x42;*/
  /*mem[0xFFFE] = 0x42;*/

  /*mem[0x4242] = CPU::INS_LDA_IM;*/
  /*mem[0x4243] = 0x12;*/

  /*mem[0x4244] = CPU::INS_STR_AZPX;*/
  /*mem[0x4245] = 0x13;*/
  /*mem[0x4244] = CPU::INS_LDA_IM;*/
  /*mem[0x4245] = 0x03;*/

  /*mem[0x4246] = CPU::INS_PHA;*/

  /*mem[0x4247] = CPU::INS_PLA;*/

  /*mem[0x4249] = CPU::INS_PLA;*/

  /*mem[0x4246] = CPU::INS_JSR;*/
  /*mem[0x4247] = 0x44;*/
  /*mem[0x4248] = 0x44;*/

  /*mem[0x4444] = CPU::INS_LDX_IM;*/
  /*mem[0x4445] = 0x02;*/
  /**/
  /*mem[0x4446] = CPU::INS_STR_AZPX;*/
  /*mem[0x4447] = 0x33;*/
  /*mem[0x0035] = 0x12;*/

  /*mem[0xFFFC] = CPU::INS_JMP_INR;*/
  /*mem[0xFFFD] = 0x34;*/
  /*mem[0xFFFE] = 0x00;*/
  /*mem[0x0034] = 0x12;*/
  /*mem[0x0035] = 0x00;*/
  /**/
  /*mem[0x0012] = CPU::INS_JMP_ABS;*/
  /*mem[0x0013] = 0x0044;*/
  /*mem[0x0014] = 0x0000;*/
  /**/
  /*mem[0x0044] = CPU::INS_STR_AZP;*/
  /*mem[0x0045] = 0x47;*/
  /*mem[0x0047] = 0x32;*/

  mem[0x4242] = CPU::INS_LDA_ZP;
  mem[0x4243] = 0x42;
  mem[0x0042] = 0x99;

  mem[0xFFFC] = CPU::INS_JSR;
  mem[0xFFFD] = 0x42;
  mem[0xFFFE] = 0x42;

  mem[0x4244] = CPU::INS_PHP;

  mem[0x4245] = CPU::INS_PLP;

  mem[0x4246] = CPU::INS_NOP;

  mem[0x4247] = CPU::INS_LDA_IM;
  mem[0x4248] = 0x05;


  mem[0x4249] = CPU::INS_ORA_IM;
  mem[0x4250] = 0x03;

  u32 Cycles = LoadCycles(mem, ci, 0xFFFC);

  clock_t start = clock();
  
  // cpu.Execute(Cycles, mem);

  clock_t end = clock();

  GUInterface.EventLoop(cpu, Cycles, mem, ci); 

  printf("ACC: %hxx %i\n", cpu.ACC, cpu.ACC); 
  double elapsed_time = double(end - start) / CLOCKS_PER_SEC;

  printf("Elapsed time: %i nanoseconds \n", int(elapsed_time * 1e9));

  GUInterface.CleanUp();

  return GUInterface.Exit();
}
