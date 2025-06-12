#include <sstream>
#include <stdexcept>
#include "core/cpu/cpu.h"

CPU::CPU(GB *gb) : gb(gb) {

  PC = 0x0000;
  AF.full = BC.full = DE.full = HL.full = SP = 0xbeef;
  cycles = 0;
  op = 0;

  print_enabled = true; // for debugging purposes

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
    if(halt_bug) {
      PC--;
      halt_bug = false;
    }

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

void CPU::read_ins() {
  op = read8(PC++);
}

bool CPU::get_flag(uint8_t mask) {
  return AF.lo & ~mask;
}

void CPU::set_flag(uint8_t mask, bool flag_val) {
  if (flag_val) AF.lo = AF.lo | ~mask;
  else AF.lo = AF.lo & mask;
}

CPUState CPU::get_state() {
  CPUState state;
  state.AF.full = AF.full;
  state.BC.full = BC.full;
  state.DE.full = DE.full;
  state.HL.full = HL.full;
  state.SP = SP;
  state.PC = PC;
  return state;
}

void CPU::set_state(CPUState state) {
  AF.full = state.AF.full;
  BC.full = state.BC.full;
  DE.full = state.DE.full;
  HL.full = state.HL.full;
  SP = state.SP;
  PC = state.PC;
}
