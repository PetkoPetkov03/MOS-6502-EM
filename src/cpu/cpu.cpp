#include "./cpu.hpp"
#include "../memory/memory.hpp"
#include "../types/types.hpp"
#include <stdio.h>
#include <system_error>
#include <unordered_map>

void CPU::Reset(Mem &memory) {
  PC = 0xFFFC;
  SP = 0x099;
  D = 0;
  ACC = 0;

  C = Z = I = D = B = O = N = 0;
  RegisterX = 0;
  RegisterY = 0;

  memory.Init();
}

Byte CPU::Fetch(u32 &Cycles, Mem &memory) {
  Byte Data = memory[PC];
  PC++;
  Cycles--;

  return Data;
}

Word CPU::FetchWord(u32 &Cycles, Mem &memory) {
  Word Data = Fetch(Cycles, memory);

  Data |= (Fetch(Cycles, memory) << 8);

  return Data;
}

Byte CPU::ReadByte(u32 &Cycles, Byte Address, Mem &memory) {
  Byte Data = memory[Address];

  Cycles--;

  return Data;
}

Word CPU::ReadWord(u32 &Cycles, Word Address, Mem &memory) {
  Word Data = ReadByte(Cycles, Address, memory);

  if ((Address & 0x00FF) == 0x00FF) {
    Data |= (ReadByte(Cycles, Address & 0xFF00, memory) << 8);
  } else {
    Data |= (ReadByte(Cycles, Address + 1, memory) << 8);
  }

  return Data;
}

void CPU::PushByte(Byte value, u32 &Cycles, Mem &memory) {
  memory[0x0100 + SP] = value;
  SP--;
  Cycles -= 2;
}

Byte CPU::PullByte(u32 &Cycles, Mem &memory) {
  SP++;
  Byte value = memory[0x0100 + SP];

  Cycles -= 2;
  return value;
}

void CPU::SetImmediate(u32 &Cycles, Byte &Register, Mem &memory) {
  Byte Value = Fetch(Cycles, memory);

  Register = Value;
}

void CPU::SetZeroPage(u32 &Cycles, Byte &Register, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  CheckZPOverflow(ZeroPageAddress);

  Register = ReadByte(Cycles, ZeroPageAddress, memory);
}

void CPU::SetZeroPageByRegister(u32 &Cycles, Byte &Register, Byte IncRegister,
                                Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  ZeroPageAddress += IncRegister;
  CheckZPOverflow(ZeroPageAddress);
  Cycles--;

  Register = ReadByte(Cycles, ZeroPageAddress, memory);
}

void CPU::SetAbsolute(u32 &Cycles, Byte &Register, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  Register = ReadByte(Cycles, AbsoluteAddress, memory);
}

void CPU::SetAbsoluteByRegister(u32 &Cycles, Byte &Register, Byte IncRegister,
                                Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);
  Word EffectiveAddress = AbsoluteAddress + IncRegister;

  Register = ReadByte(Cycles, EffectiveAddress, memory);

  if ((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) {
    Cycles--;
  }
}

// check zero page overflow
void CPU::CheckZPOverflow(Byte Address) {
  assert(Address <= 0xFF && "Address is out of zero-page range");
}

void CPU::SetStatusZN(Byte Register) {
  Z = (Register == 0);
  N = (Register & (1 << 7)) > 0;
}

void CPU::ZeroPageWrapAround(u32 &Cycles, Byte &Address) {
  Address = Address & 0xFF;

  Cycles--;
}

void CPU::LoadFlags(u32 &Cycles, Byte Flags) {
  Z = Flags >> 7;
  O = Flags >> 6;
  B = Flags >> 4;
  D = Flags >> 3;
  I = Flags >> 2;
  Z = Flags >> 1;
  C = Flags;

  Cycles--;
}

void CPU::Execute(u32 Cycles, Mem &memory) {
  while (Cycles > 0) {
    Byte Instruction = Fetch(Cycles, memory);

    printf("%hhx\n", Instruction);

    switch (Instruction) {
    case INS_PHA: {
      PushByte(ACC, Cycles, memory);
      printf("ACC Pushed on stack\n");
    } break;

    case INS_PHP: {
      Byte sf = (Z << 7) | (O << 6) | (1 << 5) | (B << 4) | (D << 3) |
                (I << 2) | (Z << 1) | C;

      PushByte(sf, Cycles, memory);

      printf("Flags Pushed on stack %hhx \n", sf);
    } break;

    case INS_PLA: {
      Byte Value = PullByte(Cycles, memory);
      ACC = Value;
      Cycles--;

      SetStatusZN(ACC);
      printf("Pull Acc %hhx\n", Value);
    }

    case INS_PLP: {
      Byte Flags = PullByte(Cycles, memory);

      LoadFlags(Cycles, Flags);

      printf("Z %i N %i O %i B %i D %i I %i Z %i C %i\n", Z, N, O, B, D, I, Z,
             C);
    } break;

    case INS_LDA_IM: {
      SetImmediate(Cycles, ACC, memory);
      SetStatusZN(ACC);
      printf("Imidiate BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
    } break;

    case INS_LDA_ZP: {
      SetZeroPage(Cycles, ACC, memory);
      SetStatusZN(ACC);
      printf("ZeroPageAddress BV: %hhx V: %i Z: %i N: %i\n", ACC, ACC, Z, N);
    } break;

    case INS_LDA_ZPX: {
      SetZeroPageByRegister(Cycles, ACC, RegisterX, memory);
      SetStatusZN(ACC);

      printf("ZeroPageAddressX  BV: %hhx V: %i Z: %i N: %i X: %hhx \n", ACC,
             ACC, Z, N, RegisterX);
    } break;

    case INS_LDA_ABS: {
      SetAbsolute(Cycles, ACC, memory);

      SetStatusZN(ACC);
      printf("LDA ABS ACC: %hhx Z: %i N: %i \n", ACC, Z, N);
    } break;

    case INS_LDA_ABSX: {
      SetAbsoluteByRegister(Cycles, ACC, RegisterX, memory);

      SetStatusZN(ACC);
    } break;

    case INS_LDA_ABSY: {
      SetAbsoluteByRegister(Cycles, ACC, RegisterY, memory);

      SetStatusZN(ACC);
    } break;

    case INS_LDA_INRX: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);

      ZeroPageAddress += RegisterX;

      ZeroPageAddress = ZeroPageAddress & 0xFF;
      Cycles--;

      Word EffectiveAddress = ReadWord(Cycles, ZeroPageAddress, memory);

      ACC = ReadByte(Cycles, EffectiveAddress, memory);

      SetStatusZN(ACC);
    } break;

    case INS_LDA_INRY: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);

      Byte HighByte = ReadByte(Cycles, ZeroPageAddress + 1, memory);

      Byte FullAddress = ZeroPageAddress;
      FullAddress |= HighByte;

      FullAddress += RegisterY;

      Word EffectiveAddress = ReadWord(Cycles, FullAddress, memory);

      ACC = ReadByte(Cycles, EffectiveAddress, memory);

      if ((FullAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) {
        Cycles--;
      }

      SetStatusZN(ACC);
    } break;

    case INS_JSR: {
      Word SubAddr = FetchWord(Cycles, memory);
      memory.WriteWord(Cycles, PC - 1, SP);
      PC++;
      PC = SubAddr;
      Cycles--;
      printf("Jump absolute %hu BV: %hhx V: %i Z: %i N: %i X: %hhx \n", SP, ACC,
             ACC, Z, N, RegisterX);

    } break;

    case INS_JMP_ABS: {
      Word SubAddr = FetchWord(Cycles, memory);
      PC = SubAddr;

      printf("JMP ABS PC: %hu\n", PC);
    } break;

    case INS_JMP_INR: {
      Word SubAddr = FetchWord(Cycles, memory);

      Word TargetAddress = ReadWord(Cycles, SubAddr, memory);

      PC = TargetAddress;

      printf("JMP IND SUB: %hu TA: %hu PC: %hu\n", SubAddr, TargetAddress, PC);
    } break;

    case INS_LDX_IM: {
      SetImmediate(Cycles, RegisterX, memory);

      SetStatusZN(RegisterX);
      printf("Load X Imidiate BV: %hhx, V: %x Z: %i N: %i X: %hhx \n",
             RegisterX, RegisterX, Z, N, RegisterX);
    } break;

    case INS_LDX_ZP: {
      SetZeroPage(Cycles, RegisterX, memory);

      SetStatusZN(RegisterX);
      printf("Load X ZP BV: %hhx, V: %x Z: %i N: %i  \n", RegisterX, RegisterX,
             Z, N);

    } break;

    case INS_LDX_ZPY: {
      SetZeroPageByRegister(Cycles, RegisterX, RegisterY, memory);
      SetStatusZN(RegisterX);
    } break;

    case INS_LDX_ABS: {
      SetAbsolute(Cycles, RegisterX, memory);

      SetStatusZN(RegisterX);
      printf("Load X ABS X: %hhx, V: %x Z: %i N: %i Y: %hhx \n", RegisterX,
             RegisterX, Z, N, RegisterY);

    } break;

    case INS_LDX_ABSY: {
      SetAbsoluteByRegister(Cycles, RegisterX, RegisterY, memory);

      SetStatusZN(RegisterX);
      printf("Load X ABS X: %hhx, V: %x Z: %i N: %i Y: %hhx \n", RegisterX,
             RegisterX, Z, N, RegisterY);

    } break;

    case INS_LDY_IM: {
      SetImmediate(Cycles, RegisterY, memory);

      SetStatusZN(RegisterY);

      printf("Load Y IM Y: %hhx, V: %x Z: %i N: %i \n", RegisterY, RegisterY, Z,
             N);

    } break;

    case INS_LDY_ZP: {
      SetZeroPage(Cycles, RegisterY, memory);
      SetStatusZN(RegisterY);
    } break;

    case INS_LDY_ZPX: {
      SetZeroPageByRegister(Cycles, RegisterY, RegisterX, memory);
      SetStatusZN(RegisterY);
    } break;

    case INS_LDY_ABS: {
      SetAbsolute(Cycles, RegisterY, memory);

      SetStatusZN(RegisterY);
    } break;

    case INS_LDY_ABSX: {
      SetAbsoluteByRegister(Cycles, RegisterY, RegisterX, memory);

      SetStatusZN(RegisterY);
    } break;

    case INS_STR_AZP: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);

      Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

      memory[Value] = ACC;

      printf("STA ACC: %hhx mem: %hhx, memv: %hu\n", ACC, memory[Value],
             memory[Value]);
    } break;

    case INS_STR_AZPX: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);

      ZeroPageAddress += RegisterX;
      Cycles--;

      Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

      memory[Value] = ACC;

      printf("STAZPX ACC: %hhx mem: %hhx mem: %hu, Address %hhx \n", ACC,
             memory[Value], memory[Value], ZeroPageAddress);
    } break;

    case INS_STR_AABS: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

      memory[Value] = ACC;
    } break;

    case INS_STR_AABSX: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      AbsoluteAddress += RegisterX;

      Cycles--;

      Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

      memory[Value] = ACC;
    } break;

    case INS_STR_XZP: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);
      CheckZPOverflow(ZeroPageAddress);

      Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

      memory[Value] = RegisterX;
    } break;

    case INS_STR_XZPY: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);

      ZeroPageAddress += RegisterY;
      Cycles--;

      Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

      memory[Value] = RegisterX;
    } break;

    case INS_STR_XABS: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      Word EffectiveAddress = ReadWord(Cycles, AbsoluteAddress, memory);

      memory[EffectiveAddress] = RegisterX;
    } break;

    case INS_STR_YZP: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);

      CheckZPOverflow(ZeroPageAddress);

      Byte EffectiveAddress = ReadByte(Cycles, ZeroPageAddress, memory);

      memory[EffectiveAddress] = RegisterY;
    } break;

    case INS_STR_YZPX: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);

      ZeroPageAddress += RegisterX;
      Cycles--;

      Byte EffectiveAddress = ReadByte(Cycles, ZeroPageAddress, memory);

      memory[EffectiveAddress] = RegisterY;
    } break;

    case INS_STR_YABS: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      Word EffectiveAddress = ReadWord(Cycles, AbsoluteAddress, memory);

      memory[EffectiveAddress] = RegisterY;
    } break;

    case INS_TAX: {
      RegisterX = ACC;
      Cycles--;

      SetStatusZN(RegisterX);
    } break;

    case INS_TAY: {
      RegisterY = ACC;
      Cycles--;

      SetStatusZN(RegisterY);
    } break;

    case INS_TSX: {
      RegisterX = SP;
      Cycles--;

      SetStatusZN(RegisterX);
    } break;

    case INS_TXA: {
      ACC = RegisterX;
      Cycles--;

      SetStatusZN(ACC);
    } break;

    case INS_TXS: {
      SP = RegisterX;
      Cycles--;

      SetStatusZN(SP);

    } break;

    case INS_TYA: {
      ACC = RegisterY;
      Cycles--;

      SetStatusZN(ACC);
    } break;

    case INS_AND_IM: {
      Byte Value = Fetch(Cycles, memory);

      ACC &= Value;

      SetStatusZN(ACC);
    } break;

    case INS_AND_ZP: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);
      Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

      ACC &= Value;

      SetStatusZN(ACC);
    } break;

    case INS_AND_ZPX: {
      Byte ZeroPageAddress = Fetch(Cycles, memory);

      ZeroPageAddress += RegisterX;

      Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

      ACC &= Value;

      SetStatusZN(ACC);
    } break;

    case INS_AND_ABS: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

      ACC &= Value;

      SetStatusZN(ACC);
    } break;

    case INS_AND_ABSX: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      AbsoluteAddress += RegisterX;

      Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

      ACC &= Value;

      // CheckZPOverflow

      SetStatusZN(ACC);
    } break;

    case INS_AND_ABSY: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      AbsoluteAddress += RegisterY;

      Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

      ACC &= Value;

      SetStatusZN(ACC);
    } break;

    case INS_AND_INRX: {
      Word SubAddr = FetchWord(Cycles, memory);

      Word DirectAddress = ReadWord(Cycles, SubAddr, memory);

      DirectAddress += RegisterX;

      Byte Value = ReadByte(Cycles, DirectAddress, memory);

      ACC &= Value;

      SetStatusZN(ACC);
    } break;

    case INS_AND_INRY: {
      Word SubAddr = FetchWord(Cycles, memory);

      Word DirectAddress = ReadWord(Cycles, SubAddr, memory);

      DirectAddress += RegisterY;

      Byte Value = ReadByte(Cycles, DirectAddress, memory);

      ACC &= Value;

      SetStatusZN(ACC);
    } break;

    case INS_EOR_IM: {
      Byte Value = Fetch(Cycles, memory);

      ACC |= Value;

      SetStatusZN(ACC);
    } break;

    case INS_EOR_ZP: {
      Byte Address = Fetch(Cycles, memory);

      Byte Value = ReadByte(Cycles, Address, memory);

      ACC |= Value;

      SetStatusZN(ACC);
    } break;

    case INS_EOR_ZPX: {
      Byte Address = Fetch(Cycles, memory);

      Address += RegisterX;
      Cycles--;

      Byte Value = ReadByte(Cycles, Address, memory);

      ACC |= Value;

      SetStatusZN(ACC);
    } break;

    case INS_EOR_ABS: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

      ACC |= Value;

      SetStatusZN(ACC);
    } break;

    case INS_EOR_ABSX: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      AbsoluteAddress += RegisterX;

      Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

      ACC |= Value;

      CheckZPOverflow(ACC);
      SetStatusZN(ACC);
    } break;

    case INS_EOR_ABSY: {
      Word AbsoluteAddress = FetchWord(Cycles, memory);

      AbsoluteAddress += RegisterY;

      Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

      ACC |= Value;

      CheckZPOverflow(ACC);
      SetStatusZN(ACC);
    } break;

    case INS_EOR_INRX: {
      Word Address = FetchWord(Cycles, memory);
      Address += RegisterX;

      Word DirectAddress = ReadWord(Cycles, Address, memory);

      Byte Value = ReadByte(Cycles, DirectAddress, memory);

      ACC |= Value;

      SetStatusZN(ACC);
    } break;

    case INS_EOR_INRY: {
      Word Address = FetchWord(Cycles, memory);

      Address += RegisterY;

      Word DirectAddress = ReadWord(Cycles, Address, memory);

      Byte Value = ReadByte(Cycles, DirectAddress, memory);

      ACC |= Value;

      CheckZPOverflow(ACC);
      SetStatusZN(ACC);
    } break;

    case INS_NOP: {
      Cycles--;
      printf("No OP\n");
    } break;

    default: {
      printf("Instruction not found %hhx \n", Instruction);
      assert(false);
    } break;
    }
  }
}

u32 LoadCycles(Mem &memory, std::unordered_map<Byte, u8> I2CMap,
               std::unordered_map<Byte, u8> I2PCMap, Word start_address) {
  u32 total_cycles = 0;
  Word pc = start_address;

  while (true) {
    Byte opcode = memory[pc];
    if (I2CMap.find(opcode) != I2CMap.end()) {
      total_cycles += I2CMap[opcode];

      switch (opcode) {
      case CPU::INS_JSR: {
        Word return_address = pc + I2PCMap[opcode];

        pc = memory[pc + 1] | (memory[pc + 2] << 8);
        continue;
      } break;

      case CPU::INS_JMP_ABS: {
        pc = memory[pc + 1] | (memory[pc + 2] << 8);
        continue;
      } break;

      case CPU::INS_JMP_INR: {
        Word pointer = memory[pc + 1] | (memory[pc + 2] << 8);

        pc = memory[pointer] | (memory[pointer + 1] << 8);
        continue;
      } break;

      default: {
        pc += I2PCMap[opcode];
      } break;
      }
    } else {
      printf("LoadCycles compleate!\n");
      break;
    }
  }

  return total_cycles;
}
