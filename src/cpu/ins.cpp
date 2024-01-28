#include "../../include/CPU.h"

#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

bool CPU::LoadInsHelper(){

  // targeting x6 & xE load ins at the top
  if (op < 0x40 && (op & 0b111) == 0b110){
    temp_u8_ptr = register_operands_map[(op>>3) & 0b111]; // LHS
    // RHS is at PC
    
    if (!temp_u8_ptr){
      // (HL) is LHS
      write8(HL.full, read8(PC++));
      cycles = 3;
    }
    else{
      *temp_u8_ptr = read8(PC++);
      cycles = 2;
    }
    return true;
  }

  // targeting x2 & xA load ins at the top
  if (op < 0x40 && (op & 0b111) == 0b010){
    switch ((op>>4) & 0b11)
    {
      case 0b00:
        operand_addr = BC.full;
        break;
      
      case 0b01:
        operand_addr = DE.full;
        break;
      
      case 0b10:
        operand_addr = HL.full++;
        break;

      case 0b11:
        operand_addr = HL.full--;
        break;
    }

    if ((op>>3) & 1 == 0){
      // operand = A
      write8(operand_addr, AF.hi);
    }
    else {
      // A = operand
      AF.hi = read8(operand_addr);
    }

    cycles = 2;
    return true;
  }

  // targetting 4 rows of load ins at the middle
  if (op >= 0x40 && op < 0x80 && op != 0x76){
    /* Bits 1,2,3 decide the register being loaded from
    and Bits 4,5,6 decide those being loaded into */

    temp_u8_ptr = register_operands_map[(op>>3) & 0b111]; // LHS
    temp_u8_ptr_2 = register_operands_map[op & 0b111]; // RHS

    if (!temp_u8_ptr){
      // (HL) is the LHS
      write8(HL.full, *temp_u8_ptr_2);
      cycles = 2;
    }
    else if (!temp_u8_ptr_2){
      // (HL) is the RHS
      *temp_u8_ptr = read8(HL.full);
      cycles = 2;
    }
    else{
      *temp_u8_ptr = *temp_u8_ptr_2;
      cycles = 1;
    }
    return true;
  }
  
  // Handling remaining load ins
  switch (op)
  {
    case 0xE0:
      write8(0xFF00, AF.hi);
      write8(0xFF00, read8(0xFF00)+1); // increment (0xFF00)
      cycles = 3;
      return true;

    case 0xE2:
      write8(0xFF00, AF.hi);
      write8(0xFF00, read8(0xFF00)+BC.lo); // increment (0xFF00) by C
      cycles = 2;
      return true;

    case 0xF0:
      AF.hi = read8(0xFF00);
      write8(0xFF00, read8(0xFF00)+1); // increment (0xFF00)
      cycles = 3;
      return true;

    case 0xF2:
      AF.hi = read8(0xFF00);
      write8(0xFF00, read8(0xFF00)+BC.lo); // increment (0xFF00) by C
      cycles = 2;
      return true;

    case 0xEA:
      write8(read16(PC), AF.hi);
      PC += 2;
      cycles = 4;
      return true;

    case 0xFA:
      AF.hi = read8(read16(PC));
      PC += 2;
      cycles = 4;
      return true;
  }

  return false;
}

bool CPU::ArithmeticInsHelper(){
  // yet to be implemented
  return false;
}

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