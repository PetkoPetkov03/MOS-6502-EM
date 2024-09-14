#include <cassert>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>

#include <time.h>
#include <unordered_map>
#include <vector>

// http://www.6502.org/users/obelisk/index.html

using Byte = uint8_t;
using Word = uint16_t;

using u32 = uint32_t;
using u8 = uint8_t;

struct Mem 
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Init()
    {
        for(u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    // read 1 byte
    Byte operator[](u32 Address) const
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }

    // write 1 byte
    Byte& operator[](u32 Address) 
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }

    // Write 2 bytes
    void WriteWord( u32& Cycles, Word word, u32 Address)
    {
        Data[Address] = (word & 0xFF);
        Data[Address+1] = (word >> 8);

        Cycles-=2;
    }
};

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
    

    void Reset( Mem& memory )
    {
        PC = 0xFFFC;
        SP = 0x099;
        D = 0;
        ACC = 0;
        
        C = Z = I = D = B = O = N = 0;
        RegisterX = 0;
        RegisterY = 0;

        memory.Init();
    }

    Byte Fetch( u32& Cycles, Mem& memory )
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;

        return Data; 
    }

    Word FetchWord(u32& Cycles, Mem& memory)
    {
        Word Data = Fetch(Cycles, memory);

        Data |= (Fetch(Cycles, memory) << 8);


        return Data;
    }

    Byte ReadByte(u32& Cycles, Byte Address,Mem& memory) 
    {
        Byte Data = memory[Address];

        Cycles--;

        return Data;
    }

    Word ReadWord(u32& Cycles, Word Address, Mem& memory)
    {
        Word Data = ReadByte(Cycles, Address, memory);
        
        if((Address & 0x00FF) == 0x00FF) {
            Data |= (ReadByte(Cycles, Address & 0xFF00, memory) << 8);
        }else {
            Data |= (ReadByte(Cycles, Address + 1, memory) << 8);
        }

        return Data;
    }

    static constexpr Byte 
      INS_LDA_IM = 0xA9, 
      INS_LDA_ZP = 0xA5,
      INS_LDA_ZPX = 0xB5,
      INS_LDA_ABS = 0xAD,
      INS_LDA_ABSX = 0xBD,
      INS_LDA_ABSY = 0xB9,
      INS_LDA_INRX = 0xA1,
      INS_LDA_INRY = 0xB1,
      INS_LDX_IM = 0xA2,
      INS_LDX_ZP = 0xA6,
      INS_LDX_ZPY = 0xB6,
      INS_LDX_ABS = 0xAE,
      INS_LDX_ABSY = 0xBE,
      INS_LDY_IM = 0xA0,
      INS_LDY_ZP = 0xA4,
      INS_LDY_ZPX = 0xB4,
      INS_LDY_ABS = 0xAC,
      INS_LDY_ABSX = 0xBC,
      INS_JMP_ABS = 0x4C,
      INS_JMP_INR = 0x6C,
      INS_JSR = 0x20,
      INS_STR_AZP = 0x85,
      INS_STR_AZPX = 0x95,
      INS_STR_AABS = 0x8D,
      INS_STR_AABSX = 0x9D,
      INS_STR_AABSY = 0x99,
      INS_STR_XZP = 0x86,
      INS_STR_XZPY = 0x96,
      INS_STR_XABS = 0x8E,
      INS_STR_YZP = 0x84,
      INS_STR_YZPX = 0x94,
      INS_STR_YABS = 0x8C,
      INS_TAX = 0xAA,
      INS_TAY = 0xA8,
      INS_TSX = 0xBA,
      INS_TXA = 0x8A,
      INS_TXS = 0x9A,
      INS_TYA = 0x98;

    void SetImmediate(u32& Cycles, Byte& Register, Mem& memory)
    {
        Byte Value = Fetch(Cycles, memory);

        Register = Value;
    }

    void SetZeroPage(u32& Cycles, Byte& Register, Mem& memory)
    {
        Byte ZeroPageAddress = Fetch(Cycles, memory);

        CheckZPOverflow(ZeroPageAddress);

        Register = ReadByte(Cycles, ZeroPageAddress, memory);
    }

    void SetZeroPageByRegister(u32& Cycles, Byte& Register, Byte IncRegister, Mem& memory)
    {
        Byte ZeroPageAddress = Fetch(Cycles, memory);

        ZeroPageAddress += IncRegister;
        CheckZPOverflow(ZeroPageAddress);
        Cycles--;

        Register = ReadByte(Cycles, ZeroPageAddress, memory);
    }

    void SetAbsolute(u32& Cycles, Byte& Register, Mem& memory)
    {
        Word AbsoluteAddress = FetchWord(Cycles, memory);

        Register = ReadByte(Cycles, AbsoluteAddress, memory);
    }

    void SetAbsoluteByRegister(u32& Cycles, Byte& Register, Byte IncRegister, Mem& memory)
    {
        Word AbsoluteAddress = FetchWord(Cycles, memory);
        Word EffectiveAddress = AbsoluteAddress + IncRegister;

        Register = ReadByte(Cycles, EffectiveAddress, memory);

        if((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) {
                        Cycles--;
        }
    }
    
    // check zero page overflow
    void CheckZPOverflow(Byte Address)
    {
        assert(Address <= 0xFF && "Address is out of zero-page range");
    }

    void SetStatusZN(Byte Register)
    {
        Z = (Register == 0);
        N = (Register & (1 << 7)) > 0;
    }

    void ZeroPageWrapAround(u32& Cycles, Byte& Address)
    {
        Address = Address & 0xFF; 

        Cycles--;
    }

    void Execute( u32 Cycles, Mem& memory )
    {
        while (Cycles > 0)
        {
            Byte Instruction = Fetch( Cycles, memory );

            printf("%hhx\n", Instruction);

            switch (Instruction)
            {
                case INS_LDA_IM:
                {
                    SetImmediate(Cycles, ACC, memory);
                    SetStatusZN(ACC);
                    printf("Imidiate BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
                }break;

                case INS_LDA_ZP:
                {
                    SetZeroPage(Cycles, ACC, memory);
                    SetStatusZN(ACC);
                    printf("ZeroPageAddress BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
                }break;

                case INS_LDA_ZPX:
                {
                    SetZeroPageByRegister(Cycles, ACC, RegisterX, memory);
                    SetStatusZN(ACC);

                    printf("ZeroPageAddressX  BV: %hhx V: %i Z: %i N: %i X: %hhx \n", ACC, ACC, Z, N, RegisterX);
                }break;

                case INS_LDA_ABS:
                {
                    SetAbsolute(Cycles, ACC, memory);

                    SetStatusZN(ACC);
                    printf("LDA ABS ACC: %hhx Z: %i N: %i \n", ACC, Z, N);
                }break;

                case INS_LDA_ABSX:
                {
                    SetAbsoluteByRegister(Cycles, ACC, RegisterX, memory);

                    SetStatusZN(ACC);
                }break;

                case INS_LDA_ABSY:
                {
                    SetAbsoluteByRegister(Cycles, ACC, RegisterY, memory);

                    SetStatusZN(ACC);
                }break;

                case INS_LDA_INRX:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    ZeroPageAddress += RegisterX;

                    ZeroPageAddress = ZeroPageAddress & 0xFF;
                    Cycles--;

                    Word EffectiveAddress = ReadWord(Cycles, ZeroPageAddress, memory);

                    ACC = ReadByte(Cycles, EffectiveAddress, memory);
                    
                    SetStatusZN(ACC);
                }break;

                case INS_LDA_INRY:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    Byte HighByte = ReadByte(Cycles, ZeroPageAddress+1, memory);

                    Byte FullAddress = ZeroPageAddress;
                    FullAddress |= HighByte;

                    FullAddress += RegisterY;

                    Word EffectiveAddress = ReadWord(Cycles, FullAddress, memory);

                    ACC = ReadByte(Cycles, EffectiveAddress, memory);

                    if((FullAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) 
                    {
                        Cycles--;
                    }

                    SetStatusZN(ACC);
                }break;

                case INS_JSR:
                {
                    Word SubAddr =  FetchWord(Cycles, memory);
                    memory.WriteWord(Cycles, PC -1, SP);
                    PC++;
                    PC = SubAddr;
                    Cycles--;
                    printf("Jump absolute %hu BV: %hhx V: %i Z: %i N: %i X: %hhx \n",  SP, ACC, ACC, Z, N, RegisterX);

                }break;


                case INS_JMP_ABS:
                {
                   Word SubAddr = FetchWord(Cycles, memory);
                   PC = SubAddr;

                   printf("JMP ABS PC: %hu\n", PC);
                }break;

                case INS_JMP_INR:
                {
                    Word SubAddr = FetchWord(Cycles, memory);

                    Word TargetAddress = ReadWord(Cycles, SubAddr, memory);

                    PC = TargetAddress;

                    printf("JMP IND SUB: %hu TA: %hu PC: %hu\n", SubAddr, TargetAddress, PC);
                }break;

                case INS_LDX_IM:
                {
                    SetImmediate(Cycles, RegisterX, memory);

                    SetStatusZN(RegisterX);
                    printf("Load X Imidiate BV: %hhx, V: %x Z: %i N: %i X: %hhx \n", RegisterX, RegisterX, Z, N , RegisterX);
                }break;

                case INS_LDX_ZP:
                {
                    SetZeroPage(Cycles, RegisterX, memory);

                    SetStatusZN(RegisterX);
                    printf("Load X ZP BV: %hhx, V: %x Z: %i N: %i  \n", RegisterX, RegisterX, Z, N);

                }break;

                case INS_LDX_ZPY:
                {
                    SetZeroPageByRegister(Cycles, RegisterX, RegisterY, memory);
                    SetStatusZN(RegisterX);
                }break;

                case INS_LDX_ABS:
                {
                    SetAbsolute(Cycles, RegisterX, memory);

                    SetStatusZN(RegisterX);
                    printf("Load X ABS X: %hhx, V: %x Z: %i N: %i Y: %hhx \n", RegisterX, RegisterX, Z, N , RegisterY);

                }break;

                case INS_LDX_ABSY:
                {
                    SetAbsoluteByRegister(Cycles, RegisterX, RegisterY, memory);
                      
                    SetStatusZN(RegisterX);
                    printf("Load X ABS X: %hhx, V: %x Z: %i N: %i Y: %hhx \n", RegisterX, RegisterX, Z, N , RegisterY);

                }break;

                case INS_LDY_IM:
                {
                    SetImmediate(Cycles, RegisterY, memory);

                    SetStatusZN(RegisterY);

                    printf("Load Y IM Y: %hhx, V: %x Z: %i N: %i \n", RegisterY, RegisterY, Z, N);

                }break;

                case INS_LDY_ZP:
                {
                    SetZeroPage(Cycles, RegisterY, memory);
                    SetStatusZN(RegisterY);
                }break;

                case INS_LDY_ZPX:
                {
                    SetZeroPageByRegister(Cycles, RegisterY, RegisterX, memory);
                    SetStatusZN(RegisterY);
                }break;

                case INS_LDY_ABS:
                {
                    SetAbsolute(Cycles, RegisterY, memory);

                    SetStatusZN(RegisterY);
                }break;

                case INS_LDY_ABSX:
                {
                    SetAbsoluteByRegister(Cycles, RegisterY, RegisterX, memory);

                    SetStatusZN(RegisterY);
                }break;

                case INS_STR_AZP:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

                    memory[Value] = ACC;

                    printf("STA ACC: %hhx mem: %hhx, memv: %hu\n", ACC, memory[Value], memory[Value]);
                }break;

                case INS_STR_AZPX:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    ZeroPageAddress += RegisterX;
                    Cycles--;

                    Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

                    memory[Value] = ACC;

                    printf("STAZPX ACC: %hhx mem: %hhx mem: %hu, Address %hhx \n", ACC, memory[Value], memory[Value], ZeroPageAddress);
                }break;

                case INS_STR_AABS:
                {
                    Word AbsoluteAddress = FetchWord(Cycles, memory);
                    
                    Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

                    memory[Value] = ACC;
                }break;

                case INS_STR_AABSX:
                {
                    Word AbsoluteAddress = FetchWord(Cycles, memory);

                    AbsoluteAddress += RegisterX;

                    Cycles--;

                    Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

                    memory[Value] = ACC;
                }break;

                case INS_STR_XZP:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);
                    CheckZPOverflow(ZeroPageAddress);

                    Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

                    memory[Value] = RegisterX;
                }break;

                case INS_STR_XZPY:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    ZeroPageAddress += RegisterY;
                    Cycles--;

                    Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

                    memory[Value] = RegisterX;
                }break;

                case INS_STR_XABS:
                {
                    Word AbsoluteAddress = FetchWord(Cycles, memory);

                    Word EffectiveAddress = ReadWord(Cycles, AbsoluteAddress, memory);

                    memory[EffectiveAddress] = RegisterX;
                }break;

                case INS_STR_YZP:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    CheckZPOverflow(ZeroPageAddress);

                    Byte EffectiveAddress = ReadByte(Cycles, ZeroPageAddress, memory);

                    memory[EffectiveAddress] = RegisterY;
                }break;


                case INS_STR_YZPX:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    ZeroPageAddress += RegisterX;
                    Cycles--;

                    Byte EffectiveAddress = ReadByte(Cycles, ZeroPageAddress, memory);

                    memory[EffectiveAddress] = RegisterY;
                }break;

                case INS_STR_YABS:
                {
                    Word AbsoluteAddress = FetchWord(Cycles, memory);

                    Word EffectiveAddress = ReadWord(Cycles, AbsoluteAddress, memory);

                    memory[EffectiveAddress] = RegisterY;
                }break;

                case INS_TAX:
                {
                    RegisterX = ACC;
                    Cycles--;

                    SetStatusZN(RegisterX);
                }break;

                case INS_TAY:
                {
                    RegisterY = ACC;
                    Cycles--;

                    SetStatusZN(RegisterY);
                }break;

                case INS_TSX:
                {
                  RegisterX = SP;
                  Cycles--;

                  SetStatusZN(RegisterX);
                }break;

                case INS_TXA:
                {
                    ACC = RegisterX;
                    Cycles--;

                    SetStatusZN(ACC);
                }break;

                case INS_TXS:
                {
                    SP = RegisterX;
                    Cycles--;

                    SetStatusZN(SP);

                }break;

                case INS_TYA:
                {
                   ACC = RegisterY;
                   Cycles--;

                   SetStatusZN(ACC);
                }break;

                default: 
                {
                    printf("Instruction not found %hhx \n", Instruction);
                    assert(false);
                }break;
            }
        }
    }
};

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

int main()
{
    clock_t start = clock();
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

    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;

    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x32;

    mem[0x4244] = CPU::INS_STR_AZPX;
    mem[0x4245] = 0x00;
    mem[0x4246] = 0x13;


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

    u32 Cycles = LoadCycles(mem, I2CMap, I2PCMap, 0xFFFC);

    cpu.Execute(Cycles, mem);

    clock_t end = clock();

    double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    
    printf("Elapsed time: %i nanoseconds \n", int(elapsed_time*1e9));

    return 0;
}
