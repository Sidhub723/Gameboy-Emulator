#include "../../include/CPU.h"


#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

//SECTION - PREFIX INSTRUCTIONS

void CPU::PFX()
{
  read_ins();

  
  if (op & 0b11000000 != 0b00000000){ // Check if first 2 bits are 11
    pfx_register_index = op & 0b00000111;
    pfx_bit_index = (op & 0b00111000) >> 3;
    
    if(pfx_register_index == 0b110){  // HL
      PFX_SET_HL();
      cycles = (3+1);
    }

    if(pfx_register_index != 0b110){
      pfx_working_register = pfx_register_operands_map[pfx_register_index];
      PFX_SET_R8();
      cycles = (1+1);
    }

  }

    if(op & 0b01000000 & ~( op & 0b10000000)){ //checking for bits starting from 01 and avoiding an 11 match

        pfx_bit = (op & 0b00111000) >> 3;
        pfx_bit_reg= pfx_register_operands_map[ (op & 0b00000111) ] ;

        if( (op & 0b00000111) == 0b110 ){ // Operand Register HL check with 6 as for BIT Condition HL is 6 as in it's column in the prefix table
            PFX_BIT_HL();
            CPU::cycles = (3+1);
            return;
        } 

        if( (op & 0b00000111) != 0b110 ){
            PFX_BIT();
            cycles = (1+1); //The prefix actually takes 1 (4/4) cycle to execute, so we need to add one to the instruction latency of these instructions while assigning the value to cycles
            return;
        }

    }

    uint8_t prefix_family = op & 0b11000000;
    uint8_t exact_function = op & 0b00111000;
    uint8_t register_family = op & 0b00000111;
    if(prefix_family == 0b00)
    {
        if(exact_function == 0b100) 
        {
        if(register_family == 0b100 || register_family == 0b101)
            CPU::SLA_HL();
        else
            CPU::SLA();

        }
    }

    if ((op ^ 0b10000000) <= 0b00111111) //Check if first 2 bits are 10
    {
        pfx_register_index = op & 0b00000111;
        pfx_bit_index = (op & 0b00111000) >> 3;

        if (pfx_register_index == 0b110) //HL
        {
        PFX_RES_HL();
        cycles = (3+1);
        }

        if (pfx_register_index != 0b110) //not (HL)
        {
        pfx_working_register = pfx_register_operands_map[pfx_register_index];
        PFX_RES_R8();
        cycles = (1+1);
        }
    

  }

}

void CPU::PFX_SET_HL(){
    operand = read8(HL.full);
    operand_addr = HL.full;
    operand |= (1 << pfx_bit_index);
    write8(operand_addr, operand);

  }

void CPU::PFX_SET_R8(){
  *pfx_working_register |= (1 << pfx_bit_index);
}


void CPU::PFX_BIT_HL(){ //here we will use the CPU::HL Register

    operand_addr=HL.full;
    operand = read8(HL.full);
    set_flag(Flags::zero, ~( operand & ( 1<<pfx_bit ) ));
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 1);
    
}

void CPU::PFX_BIT(){

    set_flag(Flags::zero, ~( *pfx_bit_reg & ( 1<<pfx_bit ) ));
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
  *pfx_working_register &= ~(1 << pfx_bit_index);
}

void CPU::SLA()
{
  uint8_t register_family = op & 0b00000111;
  uint8_t *reg = pfx_register_operands_map[register_family];
  cycles = (1+1);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if((*reg) & 0b10000000)
    set_flag(Flags::carry, 1);
  else
    set_flag(Flags::carry, 0);
  *reg = (*reg<<1);
  if(*reg==0)
    set_flag(Flags::zero, 1);

}

void CPU::SLA_HL()
{
  uint8_t register_family = op & 0b00000111;
  uint8_t *reg = pfx_register_operands_map[register_family];
  cycles = (3+1);
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if((*reg) & 0b10000000)
    set_flag(Flags::carry, 1);
  else
    set_flag(Flags::carry, 0);
  *reg = (*reg<<1);
  if(*reg==0)
    set_flag(Flags::zero, 1);
  
}
