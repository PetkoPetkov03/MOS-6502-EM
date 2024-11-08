#include "./cpu.hpp"
#include "../memory/memory.hpp"
#include "../types/types.hpp"
#include "ci.hpp"
#include "opimpl.cpp"
#include "ci.cpp"
#include <cstdio>
#include <iostream>

void CPU::Reset(Mem &memory) {
  PC = 0xFFFC;
  SP = 0x099;
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

void CPU::SetSTRZP(u32 &Cycles, Byte Register, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

  memory[Value] = Register;
}

void CPU::SetSTRZPRegister(u32 &Cycles, Byte Register, Byte IncRegister,
                           Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  ZeroPageAddress += IncRegister;
  Cycles--;

  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

  memory[Value] = Register;
}

void CPU::SetSTRABS(u32 &Cycles, Byte Register, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  memory[Value] = Register;
}

void CPU::SetSTRABSRegister(u32 &Cycles, Byte Register, Byte IncRegister,
                            Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  AbsoluteAddress += IncRegister;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  memory[Value] = Register;
}

void CPU::InvokeNOP(u32 &Cycles) { Cycles--; }

void CPU::SetCMPIM(u32 &Cycles, Mem &memory) {
  Byte Value = Fetch(Cycles, memory);
  Byte Result = ACC - Value;

  if (Result >= 0) {
    C = 1;
    if (Result == 0) {
      Z = 1;
    }
  }

  if ((Result & (1 << 7)) > 0) {
    N = 1;
  }
}

void CPU::SetCMPZP(u32 &Cycles, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

  Byte Result = ACC - Value;

  if (Result >= 0) {
    C = 1;

    if (Result == 0) {
      Z = 1;
    }
  }

  if ((Result & (1 << 7)) > 0) {
    N = 1;
  }
}

void CPU::SetCMPZPX(u32 &Cycles, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);
  ZeroPageAddress += RegisterX;

  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

  Byte Result = ACC - Value;

  if (Result >= 0) {
    C = 1;

    if (Result == 0) {
      Z = 1;
    }
  }

  if ((Result & (1 << 7)) > 0) {
    N = 1;
  }
}

void CPU::SetCMPABS(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  Byte Result = ACC - Value;

  if (Result >= 0) {
    C = 1;

    if (Result == 0) {
      Z = 1;
    }
  }

  if ((Result & (1 << 7)) > 0) {
    N = 1;
  }
}

void CPU::SetCMPABSX(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  AbsoluteAddress += RegisterX;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  Byte Result = ACC - Value;

  if (Result >= 0) {
    C = 1;

    if (Result == 0) {
      Z = 1;
    }
  }

  if ((Result & (1 << 7)) > 0) {
    N = 1;
  }
}

void CPU::SetCMPABSY(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  AbsoluteAddress += RegisterY;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  Byte Result = ACC - Value;

  C = ACC >= Value;
  Z = Result == 0;
  N = Result & (1 << 7);
}

void CPU::SetCMPINRX(u32 &Cycles, Mem &memory) {
  Word IndirectAddress = FetchWord(Cycles, memory);

  Word AbsoluteAddress = ReadWord(Cycles, IndirectAddress, memory);

  AbsoluteAddress += RegisterX;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  Byte Result = ACC - Value;

  C = ACC >= Value;
  Z = Result == 0;
  N = Result & (1 << 7);
}

void CPU::SetCMPINRY(u32 &Cycles, Mem &memory) {
  Word IndirectAddress = FetchWord(Cycles, memory);

  Word AbsoluteAddress = ReadWord(Cycles, IndirectAddress, memory);

  AbsoluteAddress += RegisterY;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  Byte Result = ACC - Value;

  C = ACC >= Value;
  Z = Result == 0;
  N = Result & (1 << 7);
}

void CPU::SetORAIM(u32 &Cycles, Mem &memory) {
  Byte Value = Fetch(Cycles, memory);

  ACC = ACC | Value;

  SetStatusZN(ACC);
}

void CPU::SetORAZP(u32 &Cycles, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

  ACC |= Value;
}

void CPU::SetORAZPX(u32 &Cycles, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  ZeroPageAddress += RegisterX;
  Cycles--;

  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

  ACC |= Value;

  SetStatusZN(ACC);
}

void CPU::SetORAABS(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC |= Value;

  SetStatusZN(ACC);
}

void CPU::SetORAABSX(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  AbsoluteAddress += RegisterX;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC |= Value;

  SetStatusZN(ACC);
}

void CPU::SetORAABSY(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  AbsoluteAddress += RegisterY;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC |= Value;

  SetStatusZN(ACC);
}

void CPU::SetORAINRX(u32 &Cycles, Mem &memory) {
  Word IndirectAddress = FetchWord(Cycles, memory);

  IndirectAddress += RegisterX;

  Word AbsoluteAddress = ReadWord(Cycles, IndirectAddress, memory);

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC |= Value;

  SetStatusZN(ACC);
}

void CPU::SetORAINRY(u32 &Cycles, Mem &memory) {
  Word IndirectAddress = FetchWord(Cycles, memory);

  IndirectAddress += RegisterY;

  Word AbsoluteAddress = ReadWord(Cycles, IndirectAddress, memory);

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC |= Value;

  SetStatusZN(ACC);
}

void CPU::SetSTRIndirectX(u32 &Cycles, Byte Register, Mem &memory) {
  Word RelativeAddress = FetchWord(Cycles, memory);

  RelativeAddress += RegisterX;
  Cycles--;

  Word AbsoluteAddress = ReadWord(Cycles, RelativeAddress, memory);

  memory[AbsoluteAddress] = Register;
}

void CPU::SetSTRIndirectY(u32 &Cycles, Byte Register, Mem &memory) {
  Word RelativeAddress = FetchWord(Cycles, memory);

  RelativeAddress += RegisterY;
  Cycles--;

  Word AbsoluteAddress = ReadWord(Cycles, RelativeAddress, memory);

  memory[AbsoluteAddress] = Register;
}

void CPU::InvokeJSR(u32 &Cycles, Mem &memory) {
  Word SubAddr = FetchWord(Cycles, memory);
  memory.WriteWord(Cycles, PC - 1, SP);
  PC++;
  PC = SubAddr;
  Cycles--;
}

void CPU::InvokeJMPABS(u32 &Cycles, Mem &memory) {
  Word SubAddr = FetchWord(Cycles, memory);
  PC = SubAddr;
}

void CPU::InvokeJMPINR(u32 &Cycles, Mem &memory) {
  Word SubAddr = FetchWord(Cycles, memory);

  Word TargetAddress = ReadWord(Cycles, SubAddr, memory);

  PC = TargetAddress;
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
  std::cout << "PUSH Stack pointer: " << static_cast<float>(SP) << std::endl;
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

  SetStatusZN(Register);
}

void CPU::SetZeroPage(u32 &Cycles, Byte &Register, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  CheckZPOverflow(ZeroPageAddress);

  Register = ReadByte(Cycles, ZeroPageAddress, memory);

  SetStatusZN(Register);
}

void CPU::SetZeroPageByRegister(u32 &Cycles, Byte &Register, Byte IncRegister,
                                Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  ZeroPageAddress += IncRegister;
  CheckZPOverflow(ZeroPageAddress);
  Cycles--;

  Register = ReadByte(Cycles, ZeroPageAddress, memory);

  SetStatusZN(Register);
}

void CPU::SetIndirectX(u32 &Cycles, Byte &Register, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  ZeroPageAddress += RegisterX;

  ZeroPageAddress = ZeroPageAddress & 0xFF;
  Cycles--;

  Word EffectiveAddress = ReadWord(Cycles, ZeroPageAddress, memory);

  Register = ReadByte(Cycles, EffectiveAddress, memory);

  SetStatusZN(Register);
}

void CPU::SetIndirectY(u32 &Cycles, Byte &Register, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);

  Byte HighByte = ReadByte(Cycles, ZeroPageAddress + 1, memory);

  Byte FullAddress = ZeroPageAddress;
  FullAddress |= HighByte;

  FullAddress += RegisterY;

  Word EffectiveAddress = ReadWord(Cycles, FullAddress, memory);

  Register = ReadByte(Cycles, EffectiveAddress, memory);

  if ((FullAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) {
    Cycles--;
  }

  SetStatusZN(Register);
}

void CPU::SetAbsolute(u32 &Cycles, Byte &Register, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  Register = ReadByte(Cycles, AbsoluteAddress, memory);

  SetStatusZN(Register);
}

void CPU::SetAbsoluteByRegister(u32 &Cycles, Byte &Register, Byte IncRegister,
                                Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);
  Word EffectiveAddress = AbsoluteAddress + IncRegister;

  Register = ReadByte(Cycles, EffectiveAddress, memory);

  if ((AbsoluteAddress & 0xFF00) != (EffectiveAddress & 0xFF00)) {
    Cycles--;
  }

  SetStatusZN(Register);
}

void CPU::TransferRegister(u32 &Cycles, Byte Register, Byte &StoreRegister,
                           Mem &memory) {
  StoreRegister = Register;
  Cycles--;

  SetStatusZN(StoreRegister);
}

void CPU::SetANDIM(u32 &Cycles, Mem &memory) {
  Byte Value = Fetch(Cycles, memory);

  ACC &= Value;

  SetStatusZN(ACC);
}

void CPU::SetANDZP(u32 &Cycles, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);
  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

  ACC &= Value;

  SetStatusZN(ACC);
}

void CPU::SetANDZPX(u32 &Cycles, Mem &memory) {
  Byte ZeroPageAddress = Fetch(Cycles, memory);
  ZeroPageAddress += RegisterX;
  Cycles--;
  Byte Value = ReadByte(Cycles, ZeroPageAddress, memory);

  ACC &= Value;

  SetStatusZN(ACC);
}

void CPU::SetANDABS(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC &= Value;

  SetStatusZN(ACC);
}

void CPU::SetANDABSX(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  // todo: implement page crossing
  AbsoluteAddress += RegisterX;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC &= Value;

  SetStatusZN(ACC);
}

void CPU::SetANDABSY(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  // todo: implement page crossing
  AbsoluteAddress += RegisterX;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC &= Value;

  SetStatusZN(ACC);
}

void CPU::SetANDINRX(u32 &Cycles, Mem &memory) {
  Word SubAddr = FetchWord(Cycles, memory);

  Word DirectAddress = ReadWord(Cycles, SubAddr, memory);

  DirectAddress += RegisterX;

  Byte Value = ReadByte(Cycles, DirectAddress, memory);

  ACC &= Value;

  SetStatusZN(ACC);
}

void CPU::SetANDINRY(u32 &Cycles, Mem &memory) {
  Word SubAddr = FetchWord(Cycles, memory);

  Word DirectAddress = ReadWord(Cycles, SubAddr, memory);

  DirectAddress += RegisterY;

  Byte Value = ReadByte(Cycles, DirectAddress, memory);

  ACC &= Value;

  SetStatusZN(ACC);
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

void CPU::SetEORIM(u32 &Cycles, Mem &memory) {
  Byte Value = Fetch(Cycles, memory);

  ACC ^= Value;

  SetStatusZN(ACC);
}

void CPU::SetEORZP(u32 &Cycles, Mem &memory) {
  Byte Address = Fetch(Cycles, memory);

  Byte Value = ReadByte(Cycles, Address, memory);

  ACC ^= Value;

  SetStatusZN(ACC);
}

void CPU::SetEORZPX(u32 &Cycles, Mem &memory) {
  Byte Address = Fetch(Cycles, memory);

  Address += RegisterX;

  Byte Value = ReadByte(Cycles, Address, memory);

  ACC ^= Value;

  SetStatusZN(ACC);
}

void CPU::SetEORABS(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC ^= Value;

  SetStatusZN(ACC);
}

void CPU::SetEORABSX(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  AbsoluteAddress += RegisterX;
  Cycles--;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC ^= Value;

  SetStatusZN(ACC);
}

void CPU::SetEORABSY(u32 &Cycles, Mem &memory) {
  Word AbsoluteAddress = FetchWord(Cycles, memory);

  AbsoluteAddress += RegisterY;
  Cycles--;

  Byte Value = ReadByte(Cycles, AbsoluteAddress, memory);

  ACC ^= Value;

  SetStatusZN(ACC);
}

void CPU::SetEORINRX(u32 &Cycles, Mem &memory) {
  Word Address = FetchWord(Cycles, memory);
  Address += RegisterX;

  Word DirectAddress = ReadWord(Cycles, Address, memory);

  Byte Value = ReadByte(Cycles, DirectAddress, memory);

  ACC ^= Value;

  SetStatusZN(ACC);
}

void CPU::SetEORINRY(u32 &Cycles, Mem &memory) {
  Word Address = FetchWord(Cycles, memory);

  Address += RegisterY;

  Word DirectAddress = ReadWord(Cycles, Address, memory);

  Byte Value = ReadByte(Cycles, DirectAddress, memory);

  ACC ^= Value;

  CheckZPOverflow(ACC);
  SetStatusZN(ACC);
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

void CPU::Execute(u32 Cycles, CycleInfo ci, Mem &memory) {
  while (Cycles > 0) {
    Byte Instruction = Fetch(Cycles, memory);

    if(ci.I2FuncMap.find(Instruction) != ci.I2FuncMap.end()) {
      ci.I2FuncMap[Instruction](*this, Cycles, memory);
    }else {
      PANIC("OOPs");
    }
  }
}
