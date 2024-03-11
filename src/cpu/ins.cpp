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
  set_flag(Flags::zero, AF.hi + operand == 0);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, (AF.hi & 0xF) + (operand & 0xF) > 0xF);
  // set_flag(Flags::carry, (uint16_t)AF.hi + (uint16_t)operand > 0xFF);
  set_flag(Flags::carry, AF.hi > 0xFF - operand);

  AF.hi += operand;
}

void CPU::ADCA() {
  uint8_t carry = get_flag(Flags::carry);
  set_flag(Flags::zero, AF.hi + operand + carry == 0);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, (AF.hi & 0xF) + (operand & 0xF) + carry > 0xF);
  // set_flag(Flags::carry, (uint16_t)AF.hi + (uint16_t)operand + carry > 0xFF);
  set_flag(Flags::carry, AF.hi > 0xFF - operand - carry);

  AF.hi += operand + carry;
}

void CPU::SUBA() {
  set_flag(Flags::zero, AF.hi - operand == 0);
  set_flag(Flags::neg, 1);
  set_flag(Flags::half_carry, (AF.hi & 0xF) - (operand & 0xF) < 0);
  set_flag(Flags::carry, AF.hi < operand);

  AF.hi -= operand;
  
}

void CPU::SBCA() {
  uint8_t carry = get_flag(Flags::carry);
  set_flag(Flags::zero, AF.hi - operand - carry == 0);
  set_flag(Flags::neg, 1);
  set_flag(Flags::half_carry, (AF.hi & 0xF) - (operand & 0xF) - carry < 0);
  set_flag(Flags::carry, AF.hi < operand + carry);

  AF.hi -= operand + carry;
}

void CPU::ANDA() {
  AF.hi = AF.hi & operand;

  set_flag(Flags::zero, AF.hi == 0);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 1);
  set_flag(Flags::carry, 0);
}

void CPU::XORA() {
  AF.hi = AF.hi ^ operand;

  set_flag(Flags::zero, AF.hi == 0);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, 0);
}

void CPU::ORA() {
  AF.hi = AF.hi | operand;
  set_flag(Flags::zero, AF.hi == 0);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, 0);
  
}

void CPU::CPA() {
  set_flag(Flags::zero, AF.hi - operand == 0);
  set_flag(Flags::neg, 1);
  set_flag(Flags::half_carry, (AF.hi & 0xF) - (operand & 0xF) < 0);
  set_flag(Flags::carry, AF.hi < operand);
}

void CPU::WRA() {
  write8(operand_addr, AF.hi);
}