#include "../../include/CPU.h"
#include "../../include/GB.h"
#include <iostream>
#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

CPU::CPU(GB *gb) : gb(gb) {

  PC = 0x0000;
  AF.full = BC.full = DE.full = HL.full = SP = 0xbeef;
  cycles = 0;
  op = 0;

  

  // setting up the instruction map
  instruction_map[0x21] = FuncDetails(&CPU::IMM16, &CPU::LDHL, 3);
  instruction_map[0x31] = FuncDetails(&CPU::IMM16, &CPU::LDSP, 3);
  instruction_map[0xaf] = FuncDetails(&CPU::IMP, &CPU::XORA, 1);
  instruction_map[0x32] = FuncDetails(&CPU::HLD, &CPU::WRA, 2);


  //PREFIX Map
  instruction_map[0xcb] = FuncDetails(&CPU::IMP, &CPU::PFX, 1);


}

void CPU::clock() {
  if (!cycles) {
    // fetch
    print_regs();
    read_ins();
    // print_regs();

    // decode
    if (instruction_map.find(op) == instruction_map.end()) {
      // print_regs();
      std::stringstream ss;
      ss << "Instruction not implemented: 0x" << std::hex << (int)op;
      throw std::runtime_error(ss.str());
    }

    // execute
    (this->*(instruction_map[op].addr_mode))();
    (this->*(instruction_map[op].ins))();
    cycles = instruction_map[op].cycles;
  }
  cycles--;
}