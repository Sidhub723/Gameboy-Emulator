#include "../../include/CPU.h"

#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

//SECTION - Addressing Modes

void CPU::IMP()
{
}

void CPU::IMM16() {
  operand = read16(PC);
  PC += 2;
}

void CPU::IMM8() {
  operand = read8(PC++);
}

void CPU::HLI() {
  operand_addr = HL.full;
  operand = read8(HL.full++);
}

void CPU::HLD() {
  operand_addr = HL.full;
  operand = read8(HL.full--);
}