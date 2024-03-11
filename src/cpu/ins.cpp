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

void CPU::LD_R16_u16() {
  uint8_t index = (op>>4) & 0b11;
  *u16_register_operands_map[index] = operand;
}

void CPU::LD_u16_SP() {
  write16(read16(PC), SP);
  PC+=2;
}

void CPU::LD_SP_HL() {
  SP = HL.full;
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

void CPU::INCR8() {
  // Not meant for (HL) case
  uint8_t index = (op>>3) & 0b111;
  (*register_operands_map[index])++;
}

void CPU::INCHL() {
  write8(HL.full, read8(HL.full)+1);
}

void CPU::DECR8() {
  // Not meant for (HL) case
  uint8_t index = (op>>3) & 0b111;
  (*register_operands_map[index])--;
}

void CPU::DECHL() {
  write8(HL.full, read8(HL.full)-1);
}

void CPU::INC16() {
  uint8_t index = (op>>4) & 0b11;
  *(u16_register_operands_map[index])++;
}

void CPU::DEC16() {
  uint8_t index = (op>>4) & 0b11;
  *(u16_register_operands_map[index])--;
}

void CPU::WRA() {
  write8(operand_addr, AF.hi);
}

// REVIEW: The last line here might cause a bug
void CPU::ADD_SP() {
  set_flag(Flags::zero, 0);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, (SP & 0xF) + (operand & 0xF) > 0xF);
  set_flag(Flags::carry, (SP & 0xFF) + (operand & 0xFF) > 0xFF);

  // sign extension of operand : operand is 8 bit, but we want to add it to a 16 bit SP so we need to sign extend it.
  SP += (int16_t)(int8_t)operand;
}

void CPU::POP() {
  uint16_t val = read16(SP);
  SP += 2;
  *u16_push_pop_register_operands_map[(op>>4) & 0b11] = val;
}

void CPU::PUSH() {
  uint16_t val = *u16_push_pop_register_operands_map[(op>>4) & 0b11];
  SP -= 2;
  write16(SP, val);
}

void CPU::RET() {
  PC = read16(SP);
  SP += 2;
}

void CPU::RETI() {
  // PC = read16(SP);
  // SP += 2;
  // gb->set_ime(true);
  std::stringstream ss;
  ss << "RETI is being called, but it is not implemented yet";
  throw std::runtime_error(ss.str());
} 

void CPU::RET_NZ() {
  if(!get_flag(Flags::zero)) {
    PC = read16(SP);
    SP += 2;
  }
}

void CPU::RET_Z() {
  if(get_flag(Flags::zero)) {
    PC = read16(SP);
    SP += 2;
  }
}

void CPU::RET_NC() {
  if(!get_flag(Flags::carry)) {
    PC = read16(SP);
    SP += 2;
  }
}

void CPU::RET_C() {
  if(get_flag(Flags::carry)) {
    PC = read16(SP);
    SP += 2;
  }
}

// REVIEW: The RST ins might not be implemented correctly
void CPU::RST() {
  SP -= 2;
  write16(SP, PC);
  PC = (op & 0b00111000);
  PC >>= 3;
  PC *= 8;
}
