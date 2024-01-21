#include "../../include/CPU.h"


#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

//SECTION - PREFIX INSTRUCTIONS

void CPU::PFX()
{
  read_ins();

  if(prefix_map.find(op) == prefix_map.end())
  {
    print_regs();
    std::stringstream ss;
    ss << "Prefix instruction not implemented: 0x" << std::hex << (int)op;
    throw std::runtime_error(ss.str());
  }

  // execute
  (this->*(prefix_map[op].addr_mode))();
  (this->*(prefix_map[op].ins))();
  cycles = prefix_map[op].cycles;
}

void CPU::PFXH()
{
  operand = HL.hi;
}

void CPU::BIT7()
{
    set_flag(Flags::zero, ~(operand & 0b10000000));
    set_flag(Flags::neg, 0);
    set_flag(Flags::half_carry, 1);
}

void CPU::BIT6()
{
    set_flag(Flags::zero, ~(operand & 0b01000000));
    set_flag(Flags::neg, 0);
    set_flag(Flags::half_carry, 1);
}

void CPU::BIT5()
{
    set_flag(Flags::zero, ~(operand & 0b00100000));
    set_flag(Flags::neg, 0);
    set_flag(Flags::half_carry, 1);
}

void CPU::BIT4()
{
    set_flag(Flags::zero, ~(operand & 0b00010000));
    set_flag(Flags::neg, 0);
    set_flag(Flags::half_carry, 1);
}

void CPU::BIT3()
{
    set_flag(Flags::zero, ~(operand & 0b00001000));
    set_flag(Flags::neg, 0);
    set_flag(Flags::half_carry, 1);
}

void CPU::BIT2()
{
    set_flag(Flags::zero, ~(operand & 0b00000100));
    set_flag(Flags::neg, 0);
    set_flag(Flags::half_carry, 1);
}

void CPU::BIT1()
{
    set_flag(Flags::zero, ~(operand & 0b00000010));
    set_flag(Flags::neg, 0);
    set_flag(Flags::half_carry, 1);
}

void CPU::BIT0()
{
    set_flag(Flags::zero, ~(operand & 0b00000001));
    set_flag(Flags::neg, 0);
    set_flag(Flags::half_carry, 1);
}