#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "./cpu/cpu.hpp"
#include "./memory/memory.hpp"
#include "types/types.hpp"
#include "./interface/interface.hpp"

// http://www.6502.org/users/obelisk/index.html

void LoadIntoMem(Mem &mem) {
  mem[0x4242] = CPU::INS_LDA_ZP;
  mem[0x4243] = 0x42;
  mem[0x0042] = 0x99;

  mem[0xFFFC] = CPU::INS_JSR;
  mem[0xFFFD] = 0x42;
  mem[0xFFFE] = 0x42;

  mem[0x4244] = CPU::INS_LDA_IM;
  mem[0x4245] = 0x05;


  mem[0x4246] = CPU::INS_JSR;
  mem[0x4247] = 0x41;
  mem[0x4248] = 0x00;
  mem[0x0041] = CPU::INS_LDX_ZP;
  mem[0x0042] = 0x32;
  mem[0x0032] = 0x03;
  mem[0x0043] = CPU::INS_PHA;
  mem[0x0044] = CPU::INS_ORA_IM;
  mem[0x0045] = 0x80;

  /*mem[0x0046] = CPU::INS_PHP;*/
  /**/
  /*mem[0x0047] = CPU::INS_LDA_IM;*/
  /*mem[0x0048] = 0x03;*/
  /**/
  /*mem[0x0049] = CPU::INS_PLP;*/
}

int VAPP(CPU& cpu, Mem& mem) {
  Interface GUInterface;

  GUInterface.InitWindow();

  LoadIntoMem(mem);
  CycleInfo ci;

  ci.Init(cpu);

  u32 Cycles = LoadCycles(mem, ci, 0xFFFC);
  printf("Cycles %i\n", Cycles);

  /*clock_t start = clock();*/
  
  /*clock_t end = clock();*/

  GUInterface.EventLoop(cpu, Cycles, mem, ci); 
  printf("%i", cpu.RegisterX);

  /*printf("ACC: %hxx %i\n", cpu.ACC, cpu.ACC); */
  /*double elapsed_time = double(end - start) / CLOCKS_PER_SEC;*/
  /**/
  /*printf("Elapsed time: %i nanoseconds \n", int(elapsed_time * 1e9));*/

  GUInterface.CleanUp();

  return GUInterface.Exit();
}

int CAPP(CPU& cpu, Mem& mem) {

  LoadIntoMem(mem);

  CycleInfo ci;
  
  ci.Init(cpu);

  u32 Cycles = LoadCycles(mem, ci,0xFFFC);

  clock_t start = clock();
  cpu.Execute(Cycles, ci, mem);
  clock_t end = clock();

  printf("ACC: %hxx %i\n", cpu.ACC, cpu.ACC); 
  double elapsed_time = double(end - start) / CLOCKS_PER_SEC;

  printf("Elapsed time: %i nanoseconds \n", int(elapsed_time * 1e9));

  return 0;
}

int main(int argv, char** argc) {
  Mem mem;
  CPU cpu;

  cpu.Reset(mem);

  int exit;

  if(argv > 1) {
    for(int i = 1; i < argv; i++) {
      if(strcmp(argc[i], "--exec=vis") == 0) {exit = VAPP(cpu, mem); break;}
      else if(strcmp(argc[i], "--exec=cons") == 0) {exit = CAPP(cpu, mem); break;}
      else PANIC("Non-valid argument given!");
    }
  }else {
    exit = VAPP(cpu, mem);
  }
 
  return exit;
}
