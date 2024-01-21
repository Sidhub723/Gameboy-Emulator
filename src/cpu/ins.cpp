#include "../../include/CPU.h"

#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

void CPU::LDSP() {
  SP = operand;
}

void CPU::LDHL() {
  HL.full = operand;
}

void CPU::XORA() {
  AF.hi = AF.hi ^ AF.hi;
  set_flag(Flags::zero, 1);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, 0);
}

void CPU::WRA() {
  write8(operand_addr, AF.hi);
}