#include "../../include/CPU.h"

#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors


// Load instructions

void CPU::LDSP() {
  SP = operand;
}

void CPU::LDHL16() {
  HL.full = operand;
}

void CPU::LDR8() {
  // Made for the 4 LOAD ins rows 
  uint8_t *tmp = register_operands_map[(op>>3) & 0b111];
  *tmp = operand;
}

void CPU::LDIMM8() {
  // made for x6 & xE LOAD ins
  uint8_t index = (op>>3) & 0b111;
  if(index == 0b110) {
    // (HL) case
    write8(HL.full, operand);
  }
  else {
    *register_operands_map[index] = operand;
  }
}

void CPU::LDHL8() {
  write8(HL.full, operand);
}

void CPU::LDA() {
  AF.hi = operand;
}

void CPU::LDBC8() {
  write8(BC.full, operand);
}

void CPU::LDDE8() {
  write8(DE.full, operand);
}

void CPU::LDHLinc() {
  write8(HL.full++, operand);
}

void CPU::LDHLdec() {
  write8(HL.full--, operand);
}

void CPU::LDU16addr() {
  write8(read16(PC), operand);
  PC+=2;
}

void CPU::LDZ1() {
  write8(0xFF00 + operand, AF.hi);
}

void CPU::LDZ2() {
  AF.hi = read8(0xFF00 + operand);
}

// Arithmetic instructions

void CPU::ADDA() {
  
}

void CPU::ADCA() {
  
}

void CPU::SUBA() {
  
}

void CPU::SBCA() {
  
}

void CPU::ANDA() {
  
}

void CPU::XORA() {
  AF.hi = AF.hi ^ AF.hi;
  set_flag(Flags::zero, 1);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, 0);
}

void CPU::ORA() {
  
}

void CPU::CPA() {
  
}

void CPU::WRA() {
  write8(operand_addr, AF.hi);
}