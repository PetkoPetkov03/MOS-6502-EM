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
    using Byte = unsigned char;
    using Word = unsigned short;

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
      INS_LDX_IM = 0xA2,
      INS_LDX_ZP = 0xA6,
      INS_LDX_ZPY = 0xB6,
      INS_LDX_ABS = 0xAE,
      INS_LDX_ABSY = 0xBE,
      INS_JMP_ABS = 0x4C,
      INS_JMP_INR = 0x6C,
      INS_JSR = 0x20;


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
                    Byte Value = Fetch(Cycles, memory);

                    ACC = Value;
                    LDASetStatus();
                    printf("Imidiate BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
                }break;

                case INS_LDA_ZP:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);
                    CheckZPOverflow(ZeroPageAddress);
                    ACC = ReadByte(Cycles, ZeroPageAddress, memory);
                    LDASetStatus();
                    printf("ZeroPageAddress BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
                }break;

                case INS_LDA_ZPX:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    ZeroPageAddress += RegisterX;
                    CheckZPOverflow(ZeroPageAddress);

                    Cycles--;

                    ACC = ReadByte(Cycles, ZeroPageAddress, memory);
                    LDASetStatus();

                    printf("ZeroPageAddressX %hhx BV: %hhx V: %i Z: %i N: %i X: %hhx \n",  ZeroPageAddress, ACC, ACC, Z, N, RegisterX);
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
                    Byte Value = Fetch(Cycles, memory);

                    RegisterX = Value;

                    LDXSetStatus();

                    printf("Load X Imidiate BV: %hhx, V: %x Z: %i N: %i X: %hhx \n", RegisterX, RegisterX, Z, N , RegisterX);
                }break;

                case INS_LDX_ZP:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);
                    CheckZPOverflow(ZeroPageAddress);

                    RegisterX = ReadByte(Cycles, ZeroPageAddress, memory);

                    LDXSetStatus();
                    printf("Load X ZP BV: %hhx, V: %x Z: %i N: %i X: %hhx \n", RegisterX, RegisterX, Z, N , ZeroPageAddress);

                }break;

                case INS_LDX_ZPY:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);
                    ZeroPageAddress += RegisterY;
                    CheckZPOverflow(ZeroPageAddress);

                    Cycles--;


                    RegisterX = ReadByte(Cycles, ZeroPageAddress, memory);
                    LDXSetStatus();
                }break;

                case INS_LDX_ABS:
                {
                    Word AbsoluteAddress = FetchWord(Cycles, memory);
                    
                    RegisterX = ReadByte(Cycles, AbsoluteAddress, memory);

                    LDXSetStatus();
                    printf("Load X ABS ABSV: %hu Y: %hhx, V: %x Z: %i N: %i Y: %hhx \n", AbsoluteAddress, RegisterX, RegisterX, Z, N , RegisterY);

                }break;

                case INS_LDX_ABSY:
                {
                    Word AbsoluteAddress = FetchWord(Cycles, memory);

                    Word EffectiveAddress = AbsoluteAddress + RegisterY;

                    RegisterX = ReadByte(Cycles, EffectiveAddress, memory);

                    LDXSetStatus();

                    if((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) {
                        Cycles--;
                    }
                    
                    printf("Load X ABS ABSV: %hu Y: %hhx, V: %x Z: %i N: %i Y: %hhx \n", EffectiveAddress, RegisterX, RegisterX, Z, N , RegisterY);

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

    mem[0x0085] = CPU::INS_LDA_IM;
    mem[0x0086] = 0x10;

    cpu.Execute(10, mem);

    return 0;
}
