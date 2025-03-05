#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "./cpu/cpu.hpp"
#include "./memory/memory.hpp"
#include "types/types.hpp"
#include "./interface/interface.hpp"

// http://www.6502.org/users/obelisk/index.html

int VAPP(CPU& cpu, Mem& mem) {
  Interface GUInterface;
  std::cout << "Hello" << std::endl;

  GUInterface.InitWindow();

  cpu.LoadIntoMem(mem);
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

  cpu.LoadIntoMem(mem);

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
      else if(strcmp(argc[i], "--help") || strcmp(argc[i], "-h")) {std::cout << "--exec=vis" << std::endl << "--exec=cons" << std::endl;}
      else PANIC("Non-valid argument given!");
    }
  }else {
    exit = VAPP(cpu, mem);
  }
 
  return exit;
}
