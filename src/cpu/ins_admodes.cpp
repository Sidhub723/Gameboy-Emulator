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

void CPU::LDfromR8() {
  // Made for the 4 LOAD ins rows
  operand = *register_operands_map[op & 0b111]; // assuming tmp != nullptr for (HL)
}

void CPU::LDfromHL8() {
  operand = read8(HL.full);
}

void CPU::LDfromA() {
  operand = AF.hi;
}

void CPU::LDfromC() {
  operand = BC.lo;
}

void CPU::LDfromBC8() {
  operand = read8(BC.full);
}

void CPU::LDfromDE8() {
  operand = read8(DE.full);
}

void CPU::LDfromU16addr() {
  operand = read8(read16(PC));
  PC+=2;
}

