#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>

// http://www.6502.org/users/obelisk/index.html

using Byte = uint8_t;
using Word = uint16_t;

using u32 = unsigned int;

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
    Byte& operator[](u32 Address) {
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
      INS_JSR = 0x20;

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
        Word EffectiveAddress = AbsoluteAddress + RegisterX;

        ACC = ReadByte(Cycles, EffectiveAddress, memory);

        if((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) {
                        Cycles--;
        }
    }
    
    // check zero page overflow
    void CheckZPOverflow(Byte Address)
    {
        assert(Address <= 0xFF && "Address is out of zero-page range");
    }

    void LDASetStatus()
    {
        Z = (ACC==0);
        N = (ACC & (1 << 7)) > 0;
    }

    void LDXSetStatus()
    {
        Z = (RegisterX==0);
        N = (RegisterX & (1 << 7)) > 0;
    }

    void LDYSetStatus()
    {
        Z = (RegisterY==0);
        N = (RegisterY & (1 << 7)) > 0;
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
                    LDASetStatus();
                    printf("Imidiate BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
                }break;

                case INS_LDA_ZP:
                {
                    SetZeroPage(Cycles, ACC, memory);
                    LDASetStatus();
                    printf("ZeroPageAddress BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
                }break;

                case INS_LDA_ZPX:
                {
                    SetZeroPageByRegister(Cycles, ACC, RegisterX, memory);
                    LDASetStatus();

                    printf("ZeroPageAddressX  BV: %hhx V: %i Z: %i N: %i X: %hhx \n", ACC, ACC, Z, N, RegisterX);
                }break;

                case INS_LDA_ABS:
                {
                    SetAbsolute(Cycles, ACC, memory);

                    LDASetStatus();
                    printf("LDA ABS ACC: %hhx Z: %i N: %i \n", ACC, Z, N);
                }break;

                case INS_LDA_ABSX:
                {
                    SetAbsoluteByRegister(Cycles, ACC, RegisterX, memory);

                    LDASetStatus();
                }break;

                case INS_LDA_ABSY:
                {
                    SetAbsoluteByRegister(Cycles, ACC, RegisterY, memory);

                    LDASetStatus();
                }break;

                case INS_LDA_INRX:
                { 
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

                    LDXSetStatus();

                    printf("Load X Imidiate BV: %hhx, V: %x Z: %i N: %i X: %hhx \n", RegisterX, RegisterX, Z, N , RegisterX);
                }break;

                case INS_LDX_ZP:
                {
                    SetZeroPage(Cycles, RegisterX, memory);

                    LDXSetStatus();
                    printf("Load X ZP BV: %hhx, V: %x Z: %i N: %i  \n", RegisterX, RegisterX, Z, N);

                }break;

                case INS_LDX_ZPY:
                {
                    SetZeroPageByRegister(Cycles, RegisterX, RegisterY, memory);
                    LDXSetStatus();
                }break;

                case INS_LDX_ABS:
                {
                    SetAbsolute(Cycles, RegisterX, memory);

                    LDXSetStatus();
                    printf("Load X ABS X: %hhx, V: %x Z: %i N: %i Y: %hhx \n", RegisterX, RegisterX, Z, N , RegisterY);

                }break;

                case INS_LDX_ABSY:
                {
                    SetAbsoluteByRegister(Cycles, RegisterX, RegisterY, memory);
                      
                    LDXSetStatus();
                    printf("Load X ABS X: %hhx, V: %x Z: %i N: %i Y: %hhx \n", RegisterX, RegisterX, Z, N , RegisterY);

                }break;

                case INS_LDY_IM:
                {
                    SetImmediate(Cycles, RegisterY, memory);

                    LDYSetStatus();

                    printf("Load Y IM Y: %hhx, V: %x Z: %i N: %i \n", RegisterY, RegisterY, Z, N);

                }break;

                case INS_LDY_ZP:
                {
                    SetZeroPage(Cycles, RegisterY, memory);
                    LDYSetStatus();
                }break;

                case INS_LDY_ZPX:
                {
                    SetZeroPageByRegister(Cycles, RegisterY, RegisterX, memory);
                    LDYSetStatus();
                }break;

                case INS_LDY_ABS:
                {
                    SetAbsolute(Cycles, RegisterY, memory);

                    LDYSetStatus();
                }break;

                case INS_LDY_ABSX:
                {
                    SetAbsoluteByRegister(Cycles, RegisterY, RegisterX, memory);

                    LDYSetStatus();
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

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);

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

    mem[0xFFFC] = CPU::INS_JMP_INR;
    mem[0xFFFD] = 0x34;
    mem[0xFFFE] = 0x00;
    mem[0x0034] = 0x12;
    mem[0x0035] = 0x00;

    mem[0x0012] = CPU::INS_JMP_ABS;
    mem[0x0013] = 0x0085;
    mem[0x0014] = 0x0000;

    mem[0x0085] = CPU::INS_LDA_ABS;
    mem[0x0086] = 0x43;
    mem[0x0087] = 0x00;

    mem[0x0043] = 0x32;

    cpu.Execute(12, mem);

    return 0;
}
