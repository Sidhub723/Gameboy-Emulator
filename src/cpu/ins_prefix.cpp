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