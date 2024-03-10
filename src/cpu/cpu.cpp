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

  //populating the prefix register operands map
  register_operands_map[0b000] = &(BC.hi);
  register_operands_map[0b001] = &(BC.lo);
  register_operands_map[0b010] = &(DE.hi);
  register_operands_map[0b011] = &(DE.lo);
  register_operands_map[0b100] = &(HL.hi);
  register_operands_map[0b101] = &(HL.lo);
  register_operands_map[0b110] = nullptr; //This is the (HL) pointer case
  register_operands_map[0b111] = &(AF.hi);

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