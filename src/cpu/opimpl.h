#ifndef OPIMPL
#define OPIMPL

#include "./cpu.hpp"

void LDA_IM(CPU &cpu, u32 &Cycles, Mem &memory);
void LDA_ZP(CPU &cpu, u32 &Cycles, Mem &memory);
void LDA_ZPX(CPU &cpu, u32 &Cycles, Mem &memory);
void LDA_ABS(CPU &cpu, u32 &Cycles, Mem &memory);
void LDA_ABSX(CPU &cpu, u32 &Cycles, Mem &memory);
void LDA_ABSY(CPU &cpu, u32 &Cycles, Mem &memory);
void LDA_INRX(CPU &cpu, u32 &Cycles, Mem &memory);
void LDA_INRY(CPU &cpu, u32 &Cycles, Mem &memory);

void LDX_IM(CPU &cpu, u32 &Cycles, Mem &memory);
void LDX_ZP(CPU &cpu, u32 &Cycles, Mem &memory);
void LDX_ZPY(CPU &cpu, u32 &Cycles, Mem &memory);
void LDX_ABS(CPU &cpu, u32 &Cycles, Mem &memory);
void LDX_ABSY(CPU &cpu, u32 &Cycles, Mem &memory);

void LDY_IM(CPU &cpu, u32 &Cycles, Mem &memory);
void LDY_ZP(CPU &cpu, u32 &Cycles, Mem &memory);
void LDY_ZPX(CPU &cpu, u32 &Cycles, Mem &memory);
void LDY_ABS(CPU &cpu, u32 &Cycles, Mem &memory);
void LDY_ABSX(CPU &cpu, u32 &Cycles, Mem &memory);

void PHA(CPU &cpu, u32 &Cycles, Mem &memory);
void PHP(CPU &cpu, u32 &Cycles, Mem &memory);
void PLA(CPU &cpu, u32 &Cycles, Mem &memory);
void PLP(CPU &cpu, u32 &Cycles, Mem &memory);

void STRAZP(CPU &cpu, u32 &Cycles, Mem &memory);
void STRAZPX(CPU &cpu, u32 &Cycles, Mem &memory);
void STRAABS(CPU &cpu, u32 &Cycles, Mem &memory);
void STRAABSX(CPU &cpu, u32 &Cycles, Mem &memory);
void STRAABSY(CPU &cpu, u32 &Cycles, Mem &memory);
void STRAINRX(CPU &cpu, u32 &Cycles, Mem &memory);
void STRAINRY(CPU &cpu, u32 &Cycles, Mem &memory);

void JSR(CPU &cpu, u32 &Cycles, Mem &memory);
void JMP_ABS(CPU &cpu, u32 &Cycles, Mem &memory);
void JMP_INR(CPU &cpu, u32 &Cycles, Mem &memory);

void TAX(CPU &cpu, u32 &Cycles, Mem &memory);
void TAY(CPU &cpu, u32 &Cycles, Mem &memory);
void TSX(CPU &cpu, u32 &Cycles, Mem &memory);
void TXA(CPU &cpu, u32 &Cycles, Mem &memory);
void TXS(CPU &cpu, u32 &Cycles, Mem &memory);
void TYA(CPU &cpu, u32 &Cycles, Mem &memory);

void AND_IM(CPU &cpu, u32 &Cycles, Mem &memory);
void AND_ZP(CPU &cpu, u32 &Cycles, Mem &memory);
void AND_ZPX(CPU &cpu, u32 &Cycles, Mem &memory);
void AND_ABS(CPU &cpu, u32 &Cycles, Mem &memory);
void AND_ABSX(CPU &cpu, u32 &Cycles, Mem &memory);
void AND_ABSY(CPU &cpu, u32 &Cycles, Mem &memory);
void AND_INRX(CPU &cpu, u32 &Cycles, Mem &memory);
void AND_INRY(CPU &cpu, u32 &Cycles, Mem &memory);

void EOR_IM(CPU &cpu, u32 &Cycles, Mem &memory);
void EOR_ZP(CPU &cpu, u32 &Cycles, Mem &memory);
void EOR_ZPX(CPU &cpu, u32 &Cycles, Mem &memory);
void EOR_ABS(CPU &cpu, u32 &Cycles, Mem &memory);
void EOR_ABSX(CPU &cpu, u32 &Cycles, Mem &memory);
void EOR_ABSY(CPU &cpu, u32 &Cycles, Mem &memory);
void EOR_INRX(CPU &cpu, u32 &Cycles, Mem &memory);
void EOR_INRY(CPU &cpu, u32 &Cycles, Mem &memory);

void ORA_IM(CPU &cpu, u32 &Cycles, Mem &memory);
void ORA_ZP(CPU &cpu, u32 &Cycles, Mem &memory);
void ORA_ZPX(CPU &cpu, u32 &Cycles, Mem &memory);
void ORA_ABS(CPU &cpu, u32 &Cycles, Mem &memory);
void ORA_ABSX(CPU &cpu, u32 &Cycles, Mem &memory);
void ORA_ABSY(CPU &cpu, u32 &Cycles, Mem &memory);
void ORA_INRX(CPU &cpu, u32 &Cycles, Mem &memory);
void ORA_INRY(CPU &cpu, u32 &Cycles, Mem &memory);

void CMP_IM(CPU &cpu, u32 &Cycles, Mem &memory);
void CMP_ZP(CPU &cpu, u32 &Cycles, Mem &memory);
void CMP_ZPX(CPU &cpu, u32 &Cycles, Mem &memory);
void CMP_ABS(CPU &cpu, u32 &Cycles, Mem &memory);
void CMP_ABSX(CPU &cpu, u32 &Cycles, Mem &memory);
void CMP_ABSY(CPU &cpu, u32 &Cycles, Mem &memory);
void CMP_INRX(CPU &cpu, u32 &Cycles, Mem &memory);
void CMP_INRY(CPU &cpu, u32 &Cycles, Mem &memory);

void NOP(CPU &cpu, u32 &Cycles, Mem &memory);
#endif // !OPIMPL
