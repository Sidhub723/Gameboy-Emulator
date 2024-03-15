#include <iostream>
#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors
#include "core/interconnect/gb.h"
#include "core/cpu/cpu.h"

CPU::CPU(GB *gb) : gb(gb) {

  PC = 0x0000;
  AF.full = BC.full = DE.full = HL.full = SP = 0xbeef;
  cycles = 0;
  op = 0;

  // populating the register maps
  initialize_register_maps();

  // setting up the instruction map
  initialize_ins_map();
}

void CPU::clock() {
  if (!cycles) {
    // fetch
    //print_regs();
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
    cycles = instruction_map[op].cycles;
    (this->*(instruction_map[op].ins))();
  }
  cycles--;
}