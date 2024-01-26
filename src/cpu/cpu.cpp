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
  pfx_register_operands_map[0b000] = &(BC.hi);
  pfx_register_operands_map[0b001] = &(BC.lo);
  pfx_register_operands_map[0b010] = &(DE.hi);
  pfx_register_operands_map[0b011] = &(DE.lo);
  pfx_register_operands_map[0b100] = &(HL.hi);
  pfx_register_operands_map[0b101] = &(HL.lo);
  pfx_register_operands_map[0b110] = nullptr; //This is the (HL) pointer case
  pfx_register_operands_map[0b111] = &(AF.hi);

  // setting up the instruction map
  instruction_map[0x21] = FuncDetails(&CPU::IMM16, &CPU::LDHL, 3);
  instruction_map[0x31] = FuncDetails(&CPU::IMM16, &CPU::LDSP, 3);
  instruction_map[0xaf] = FuncDetails(&CPU::IMP, &CPU::XORA, 1);
  instruction_map[0x32] = FuncDetails(&CPU::HLD, &CPU::WRA, 2);


  //PREFIX Map
  instruction_map[0xcb] = FuncDetails(&CPU::IMP, &CPU::PFX, -1);


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
    cycles = instruction_map[op].cycles;
    (this->*(instruction_map[op].ins))();
  }
  cycles--;
}