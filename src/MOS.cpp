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
        Word Data = memory[PC];
        PC++;

        Data |= (memory[PC] << 8);
        PC++;

        Cycles -= 2;

        return Data;
    }

    Byte ReadByte(u32& Cycles, Byte Address,Mem& memory) 
    {
        Byte Data = memory[Address];

        if(!Data) 
        {
            printf("Address out of scope!");
            assert(!Data);
        }

        Cycles--;

        return Data;
    }

    static constexpr Byte 
      INS_LDA_IM = 0xA9, 
      INS_LDA_ZP = 0xA5,
      INS_LDA_ZPX = 0xB5,
      INS_LDX_IM = 0xA2,
      INS_LDX_ZP = 0xA6,
      INS_JSR = 0x20;

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
                    
                    ACC = ReadByte(Cycles, ZeroPageAddress, memory);
                    LDASetStatus();
                    printf("ZeroPageAddress BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
                }break;

                case INS_LDA_ZPX:
                {
                    Byte ZeroPageAddress = Fetch(Cycles, memory);

                    ZeroPageAddress += RegisterX;

                    Cycles--;

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

                case INS_LDX_IM:
                {
                    Byte Value = Fetch(Cycles, memory);

                    RegisterX = Value;

                    LDXSetStatus();
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

    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    cpu.Execute(8, mem);

    return 0;
}
