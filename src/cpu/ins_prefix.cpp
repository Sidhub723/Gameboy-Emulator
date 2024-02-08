#include "../../include/CPU.h"


#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

//SECTION - PREFIX INSTRUCTIONS

void CPU::PFX()
{
  read_ins();

  pfx_ins_family_index = (op & 0b11000000) >> 6;
  pfx_bit_index = (op & 0b00111000) >> 3;
  pfx_register_index = (op & 0b00000111);
  pfx_register_ptr = register_operands_map[pfx_register_index];

  if (pfx_ins_family_index == 0b00){
    // Rotate/Shift family
    pfx_rs_family_index = (op & 0b00111000) >> 3; // equivalent to pfx_bit_index
    
    if (pfx_rs_family_index == 0b100){
      if(pfx_register_index == 0b110){
        PFX_SLA_HL();
        cycles = (3+1);
      }
      else{
        PFX_SLA_R8();
        cycles = (1+1);
      }
    }

    if (pfx_rs_family_index == 0b000){
      //RLC
      if(pfx_register_index == 0b110){
        PFX_RLC_HL();
        cycles = (3+1);
      }
      else{
        PFX_RLC_R8();
        cycles = (1+1);
      }
    }

    if (pfx_rs_family_index == 0b001){
      //RRC
      if(pfx_register_index == 0b110){
        PFX_RRC_HL();
        cycles = (3+1);
      }
      else{
        PFX_RRC_R8();
        cycles = (1+1);
      }
    }



  }
  else if (pfx_ins_family_index == 0b01){
    // BIT family
    if (pfx_register_index == 0b110){ // 0b110 corresponds to (HL) operand
      PFX_BIT_HL();
      cycles = (2+1);
    }
    else{
      PFX_BIT_R8();
      cycles = (1+1);
    }
  }
  else if (pfx_ins_family_index == 0b10){
    // RES family
    if (pfx_register_index == 0b110){ // 0b110 corresponds to (HL) operand
      PFX_RES_HL();
      cycles = (3+1);
    }
    else{
      PFX_RES_R8();
      cycles = (1+1);
    }
  }
  else if (pfx_ins_family_index == 0b11){
    // SET family
    if (pfx_register_index == 0b110){ // 0b110 corresponds to (HL) operand
      PFX_SET_HL();
      cycles = (3+1);
    }
    else{
      PFX_SET_R8();
      cycles = (1+1);
    }
  }

}

void CPU::PFX_SET_HL()
{
    operand = read8(HL.full);
    operand_addr = HL.full;
    operand |= (1 << pfx_bit_index);
    write8(operand_addr, operand);
}

void CPU::PFX_SET_R8()
{
  *pfx_register_ptr |= (1 << pfx_bit_index);
}


void CPU::PFX_BIT_HL()
{ //here we will use the CPU::HL Register
    operand_addr=HL.full;
    operand = read8(HL.full);
    set_flag(Flags::zero, ~( operand & (1 << pfx_bit_index) ));
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 1);
}

void CPU::PFX_BIT_R8()
{
    set_flag(Flags::zero, ~( *pfx_register_ptr & (1 << pfx_bit_index) ));
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 1);
}


void CPU::PFX_RES_HL()
{
  operand = read8(HL.full);
  operand_addr = HL.full;
  operand &= ~(1 << pfx_bit_index);
  write8(operand_addr, operand);
}


void CPU::PFX_RES_R8()
{
  *pfx_register_ptr &= ~(1 << pfx_bit_index);
}

void CPU::PFX_SLA_R8()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if((*pfx_register_ptr) & 0b10000000)
    set_flag(Flags::carry, 1);
  else
    set_flag(Flags::carry, 0);
  *pfx_register_ptr = (*pfx_register_ptr<<1);
  if(*pfx_register_ptr==0)
    set_flag(Flags::zero, 1);
}

void CPU::PFX_SLA_HL()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if((*pfx_register_ptr) & 0b10000000)
    set_flag(Flags::carry, 1);
  else
    set_flag(Flags::carry, 0);
  *pfx_register_ptr = (*pfx_register_ptr<<1);
  if(*pfx_register_ptr==0)
    set_flag(Flags::zero, 1);
}

void CPU::PFX_RLC_R8()
{
  uint8_t bit_7 = (*pfx_register_ptr) >> 7;
  *pfx_register_ptr <<= 1;
  *pfx_register_ptr |= bit_7;

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if (*pfx_register_ptr == 0) set_flag(Flags::zero, 1);
  set_flag(Flags::carry, bit_7);
}

void CPU::PFX_RLC_HL()
{
  operand = read8(HL.full);
  operand_addr = HL.full;
  uint8_t bit_7 = (operand) >> 7;
  operand <<= 1;
  operand |= bit_7;
  write8(operand_addr, operand);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if (*pfx_register_ptr == 0) set_flag(Flags::zero, 1);
  set_flag(Flags::carry, bit_7);
}

void CPU::PFX_RRC_R8()
{
  uint8_t bit_0 = (*pfx_register_ptr) & 1;
  *pfx_register_ptr >>= 1;
  *pfx_register_ptr |= (bit_0 << 7);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if (*pfx_register_ptr == 0) set_flag(Flags::zero, 1);
  set_flag(Flags::carry, bit_0);
}

void CPU::PFX_RRC_HL()
{
  operand = read8(HL.full);
  operand_addr = HL.full;
  uint8_t bit_0 = (operand) & 1;
  operand >>= 1;
  operand |= (bit_0 << 7);
  write8(operand_addr, operand);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if (*pfx_register_ptr == 0) set_flag(Flags::zero, 1);
  set_flag(Flags::carry, bit_0);
}