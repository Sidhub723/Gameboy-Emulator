#include "../../include/CPU.h"


#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

//SECTION - PREFIX INSTRUCTIONS

void CPU::PFX()
{
    read_ins();

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
}

void CPU::PFX_BIT_HL(){ //here we will use the CPU::HL Register

    operand_addr=HL.full;
    operand = read8(HL.full);
    operand = operand | ( 1<<pfx_bit ) ;
    write8(operand_addr, operand);
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 1);
    
}

void CPU::PFX_BIT(){

    *pfx_bit_reg = *pfx_bit_reg | ( 1<<pfx_bit ) ; // done as the pfx_bit 'th bit will be set
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 1);

}