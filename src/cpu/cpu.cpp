#include "../types/types.hpp"
#include "../memory/memory.hpp"
#include <unordered_map>
#include "./cpu.hpp"

u32 LoadCycles(Mem& memory, std::unordered_map<Byte, u8> I2CMap, std::unordered_map<Byte, u8> I2PCMap, Word start_address)
{
   u32 total_cycles = 0;
   Word pc = start_address;

   while(true) {
      Byte opcode = memory[pc];
      if(I2CMap.find(opcode) != I2CMap.end()) {
        total_cycles += I2CMap[opcode];

        switch (opcode) {
            case CPU::INS_JSR:
            {
                Word return_address = pc + I2PCMap[opcode];

                pc = memory[pc+1] | (memory[pc+2] << 8);
                continue;
            }break;
                   
            case CPU::INS_JMP_ABS:
            {
                pc = memory[pc + 1] | (memory[pc+2] << 8);
                continue;
            }break;

            case CPU::INS_JMP_INR:
            {
                Word pointer = memory[pc + 1] | (memory[pc+2] << 8);

                pc = memory[pointer] | (memory[pointer + 1] << 8);
                continue;
            }break;

            default:
            {
                pc += I2PCMap[opcode];
            }break;
        }
      }else {
        printf("LoadCycles compleate!\n");
        break;
      }
   }

   return total_cycles;
}
