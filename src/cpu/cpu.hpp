#include "../types/types.hpp"
#include "../memory/memory.hpp"
#include <unordered_map>
#include <stdio.h>

#ifndef MOS_CPU_DEF
#define MOS_CPU_DEF
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

    void PushByte(Byte value, u32& Cycles, Mem& memory) {
        memory[0x0100 + SP] = value;
        SP--;
        Cycles-=2;
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
      INS_TYA = 0x98,
      INS_PHA = 0x48,
      INS_PHP = 0x08;

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
                case INS_PHA:
                {
                    PushByte(ACC, Cycles, memory);
                    printf("ACC Pushed on stack\n");
                }break;

                case INS_PHP:
                {
                    Byte sf = (Z << 7) | (O << 6) | (1 << 5) | (B << 4) | (D << 3) | (I << 2) | (Z << 1) | C;

                    PushByte(sf, Cycles, memory);
                }break;

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


u32 LoadCycles(Mem& memory, std::unordered_map<Byte, u8> I2CMap, std::unordered_map<Byte, u8> I2PCMap, Word start_address);

#endif // !MOS_CPU_DEF

