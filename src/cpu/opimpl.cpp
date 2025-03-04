#include "./opimpl.h"

void LDA_IM(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetImmediate(Cycles, cpu.ACC, memory);
}

void LDA_ZP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetZeroPage(Cycles, cpu.ACC, memory);
}

void LDA_ZPX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetZeroPageByRegister(Cycles, cpu.ACC, cpu.RegisterX, memory);
}

void LDA_ABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetAbsolute(Cycles, cpu.ACC, memory);
}

void LDA_ABSX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetAbsoluteByRegister(Cycles, cpu.ACC, cpu.RegisterX, memory);
}

void LDA_ABSY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetAbsoluteByRegister(Cycles, cpu.ACC, cpu.RegisterY, memory);
}

void LDA_INRX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetIndirectX(Cycles, cpu.ACC, memory);
}

void LDA_INRY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetIndirectY(Cycles, cpu.ACC, memory);
}

void LDX_IM(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetImmediate(Cycles, cpu.RegisterX, memory);
}

void LDX_ZP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetZeroPage(Cycles, cpu.RegisterX, memory);
}

void LDX_ZPY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetZeroPageByRegister(Cycles, cpu.RegisterX, cpu.RegisterY, memory);
}

void LDX_ABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetAbsolute(Cycles, cpu.RegisterX, memory);
}

void LDX_ABSY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetAbsoluteByRegister(Cycles, cpu.RegisterX, cpu.RegisterY, memory);
}

void LDY_IM(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetImmediate(Cycles, cpu.RegisterY, memory);
}

void LDY_ZP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetZeroPage(Cycles, cpu.RegisterY, memory);
}

void LDY_ZPX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetZeroPageByRegister(Cycles, cpu.RegisterY, cpu.RegisterX, memory);
}

void LDY_ABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetAbsolute(Cycles, cpu.RegisterY, memory);
}

void LDY_ABSX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetAbsoluteByRegister(Cycles, cpu.RegisterY, cpu.RegisterX, memory);
}

void PHA(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.PushByte(cpu.ACC, Cycles, memory);
  printf("PHP\n");
}

void PHP(CPU &cpu, u32 &Cycles, Mem &memory) {
  Byte sf = (cpu.Z << 7) | (cpu.O << 6) | (1 << 5) | (cpu.B << 4) |
            (cpu.D << 3) | (cpu.I << 2) | (cpu.Z << 1) | cpu.C;

  cpu.PushByte(sf, Cycles, memory);
}

void PLA(CPU &cpu, u32 &Cycles, Mem &memory) {
  Byte Value = cpu.PullByte(Cycles, memory);
  cpu.ACC = Value;
  Cycles--;

  cpu.SetStatusZN(cpu.ACC);
}

void PLP(CPU &cpu, u32 &Cycles, Mem &memory) {
  Byte Flags = cpu.PullByte(Cycles, memory);

  cpu.LoadFlags(Cycles, Flags);
}

void JSR(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.InvokeJSR(Cycles, memory);
}

void JMP_ABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.InvokeJMPABS(Cycles, memory);
}

void JMP_INR(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.InvokeJMPINR(Cycles, memory);
}

void STRAZP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetSTRZP(Cycles, cpu.ACC, memory);
}

void STRAZPX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetSTRZPRegister(Cycles, cpu.ACC, cpu.RegisterX, memory);
}

void STRAABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetSTRABS(Cycles, cpu.ACC, memory);
}

void STRAABSX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetSTRABSRegister(Cycles, cpu.ACC, cpu.RegisterX, memory);
}

void STRAABSY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetSTRABSRegister(Cycles, cpu.ACC, cpu.RegisterY, memory);
}

void STRAINRX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetSTRIndirectX(Cycles, cpu.ACC, memory);
}

void STRAINRY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetSTRIndirectY(Cycles, cpu.ACC, memory);
}

void TAX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.TransferRegister(Cycles, cpu.ACC, cpu.RegisterX, memory);
}

void TAY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.TransferRegister(Cycles, cpu.ACC, cpu.RegisterY, memory);
}

void TXA(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.TransferRegister(Cycles, cpu.RegisterX, cpu.ACC, memory);
}

void TXS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.TransferRegister(Cycles, cpu.RegisterX, cpu.SP, memory);
}

void TSX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.TransferRegister(Cycles, cpu.SP, cpu.RegisterX, memory);
}

void TYA(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.TransferRegister(Cycles, cpu.RegisterY, cpu.ACC, memory);
}

void AND_IM(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetANDIM(Cycles, memory);
}

void AND_ZP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetANDZP(Cycles, memory);
}

void AND_ZPX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetANDZPX(Cycles, memory);
}

void AND_ABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetANDABS(Cycles, memory);
}

void AND_ABSX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetANDABSX(Cycles, memory);
}

void AND_ABSY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetANDABSY(Cycles, memory);
}

void AND_INRX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetANDINRX(Cycles, memory);
}

void AND_INRY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetANDINRY(Cycles, memory);
}

void EOR_IM(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORIM(Cycles, memory);
}

void EOR_ZP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORZP(Cycles, memory);
}

void EOR_ZPX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORZPX(Cycles, memory);
}

void EOR_ABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORABS(Cycles, memory);
}

void EOR_ABSX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORABSX(Cycles, memory);
}

void EOR_ABSY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORABSY(Cycles, memory);
}

void EOR_INRX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORINRX(Cycles, memory);
}

void EOR_INRY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORINRY(Cycles, memory);
}

void ORA_IM(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetORAIM(Cycles, memory);
}

void ORA_ZP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetORAZP(Cycles, memory);
}

void ORA_ZPX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetORAZPX(Cycles, memory);
}

void ORA_ABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetEORABS(Cycles, memory);
}

void ORA_ABSX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetORAABSX(Cycles, memory);
}

void ORA_ABSY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetORAABSY(Cycles, memory);
}

void ORA_INRX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetORAINRX(Cycles, memory);
}

void ORA_INRY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetORAINRY(Cycles, memory);
}

void NOP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.InvokeNOP(Cycles);
}

void CMP_IM(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetCMPIM(Cycles, memory);
}

void CMP_ZP(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetCMPZP(Cycles, memory);
}

void CMP_ZPX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetCMPZPX(Cycles, memory);
}

void CMP_ABS(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetCMPABS(Cycles, memory);
}

void CMP_ABSX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetCMPABSX(Cycles, memory);
}

void CMP_ABSY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetCMPABSY(Cycles, memory);
}

void CMP_INRX(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetCMPINRX(Cycles, memory);
}

void CMP_INRY(CPU &cpu, u32 &Cycles, Mem &memory) {
  cpu.SetCMPINRY(Cycles, memory);
}
