#include "../../include/CPU.h"


#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

//SECTION - PREFIX INSTRUCTIONS

void CPU::PFX()
{
  read_ins();


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
      cycles = (2+1);
    }
    

  }

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