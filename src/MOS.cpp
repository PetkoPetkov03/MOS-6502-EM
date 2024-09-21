#include <cassert>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>

#include <time.h>

#include "./cpu/cpu.hpp"
#include "./memory/memory.hpp"

// http://www.6502.org/users/obelisk/index.html
//

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);

    std::unordered_map<Byte, u8> I2CMap = {
      {CPU::INS_LDA_IM, 2},
      {CPU::INS_LDA_ZP, 3},
      {CPU::INS_LDA_ZPX, 4},
      {CPU::INS_LDA_ABS, 4},
      {CPU::INS_LDA_ABSX, 4},
      {CPU::INS_LDA_ABSY, 4},
      {CPU::INS_LDA_INRX, 6},
      {CPU::INS_LDA_INRY, 5},
      {CPU::INS_LDX_IM, 2},
      {CPU::INS_LDX_ZP, 3},
      {CPU::INS_LDX_ZPY, 4},
      {CPU::INS_LDX_ABS, 4},
      {CPU::INS_LDX_ABSY, 4},
      {CPU::INS_LDY_IM, 2},
      {CPU::INS_LDY_ZP, 3},
      {CPU::INS_LDY_ZPX, 4},
      {CPU::INS_LDY_ABS, 4},
      {CPU::INS_LDY_ABSX, 4},
      {CPU::INS_STR_AZP, 3},
      {CPU::INS_STR_AZPX, 4},
      {CPU::INS_STR_AABS, 4},
      {CPU::INS_STR_AABSX, 5},
      {CPU::INS_STR_AABSY, 5},
      {CPU::INS_STR_XZP, 3},
      {CPU::INS_STR_XZPY, 4},
      {CPU::INS_STR_XABS, 4},
      {CPU::INS_STR_YZP, 3},
      {CPU::INS_STR_YZPX, 4},
      {CPU::INS_STR_YABS, 4},
      {CPU::INS_TAX, 2},
      {CPU::INS_TAY, 2},
      {CPU::INS_TSX, 2},
      {CPU::INS_TXA, 2},
      {CPU::INS_TXS, 2},
      {CPU::INS_TYA, 2},
      {CPU::INS_JSR, 6},
      {CPU::INS_JMP_ABS, 3},
      {CPU::INS_JMP_INR, 5},
      {CPU::INS_PHA, 3},
      {CPU::INS_PHP, 3},
      {CPU::INS_PLA, 4},
      {CPU::INS_PLP, 4},
      {CPU::INS_AND_IM, 2},
      {CPU::INS_AND_ZP, 3},
      {CPU::INS_AND_ZPX, 4},
      {CPU::INS_AND_ABS, 4},
      {CPU::INS_AND_ABSX, 4},
      {CPU::INS_AND_ABSY, 4},
      {CPU::INS_AND_INRX, 6},
      {CPU::INS_AND_INRY, 5},
      {CPU::INS_AND_IM, 2},
      {CPU::INS_AND_ZP, 3},
      {CPU::INS_AND_ZPX, 4},
      {CPU::INS_AND_ABS, 4},
      {CPU::INS_AND_ABSX, 4},
      {CPU::INS_AND_ABSY, 4},
      {CPU::INS_AND_INRX, 6},
      {CPU::INS_AND_INRY, 5},
      {CPU::INS_NOP, 2}
    };

    std::unordered_map<Byte, u8> I2PCMap = {
      {CPU::INS_LDA_IM, 2},
      {CPU::INS_LDA_ZP, 2},
      {CPU::INS_LDA_ZPX, 2},
      {CPU::INS_LDA_ABS, 3},
      {CPU::INS_LDA_ABSX, 3},
      {CPU::INS_LDA_ABSY, 3},
      {CPU::INS_LDA_INRX, 2},
      {CPU::INS_LDA_INRY, 2},
      {CPU::INS_LDX_IM, 2},
      {CPU::INS_LDX_ZP, 2},
      {CPU::INS_LDX_ZPY, 2},
      {CPU::INS_LDX_ABS, 3},
      {CPU::INS_LDX_ABSY, 3},
      {CPU::INS_LDY_IM, 2},
      {CPU::INS_LDY_ZP, 2},
      {CPU::INS_LDY_ZPX, 2},
      {CPU::INS_LDY_ABS, 3},
      {CPU::INS_LDY_ABSX, 3},
      {CPU::INS_STR_AZP, 2},
      {CPU::INS_STR_AZPX, 2},
      {CPU::INS_STR_AABS, 3},
      {CPU::INS_STR_AABSX, 3},
      {CPU::INS_STR_AABSY, 3},
      {CPU::INS_STR_XZP, 2},
      {CPU::INS_STR_XZPY, 2},
      {CPU::INS_STR_XABS, 3},
      {CPU::INS_STR_YZP, 2},
      {CPU::INS_STR_YZPX, 2},
      {CPU::INS_STR_YABS, 3},
      {CPU::INS_TAX, 1},
      {CPU::INS_TAY, 1},
      {CPU::INS_TSX, 1},
      {CPU::INS_TXA, 1},
      {CPU::INS_TXS, 1},
      {CPU::INS_TYA, 1},
      {CPU::INS_JSR, 3},
      {CPU::INS_JMP_ABS, 3},
      {CPU::INS_JMP_INR, 3},
      {CPU::INS_PHA, 1},
      {CPU::INS_PHP, 1},
      {CPU::INS_PLA, 1},
      {CPU::INS_PLP, 1},
      {CPU::INS_AND_IM, 2},
      {CPU::INS_AND_ZP, 2},
      {CPU::INS_AND_ZPX, 2},
      {CPU::INS_AND_ABS, 3},
      {CPU::INS_AND_ABSX, 3},
      {CPU::INS_AND_ABSY, 3},
      {CPU::INS_AND_INRX, 2},
      {CPU::INS_AND_INRY, 2},
      {CPU::INS_AND_IM, 2},
      {CPU::INS_AND_ZP, 2},
      {CPU::INS_AND_ZPX, 2},
      {CPU::INS_AND_ABS, 3},
      {CPU::INS_AND_ABSX, 3},
      {CPU::INS_AND_ABSY, 3},
      {CPU::INS_AND_INRX, 2},
      {CPU::INS_AND_INRY, 2},
      {CPU::INS_NOP, 1},
    };

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

    u32 Cycles = LoadCycles(mem, I2CMap, I2PCMap, 0xFFFC);

    clock_t start = clock();
    cpu.Execute(Cycles, mem);

    clock_t end = clock();

    double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    
    printf("Elapsed time: %i nanoseconds \n", int(elapsed_time*1e9));

    return 0;
}
