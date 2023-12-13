#include "../include/CPU.h"
#include "../include/GBA.h"
#include <iostream>
#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

CPU::CPU(GBA *gba) : gba(gba) {

  PC = 0x0000;
  AF.full = BC.full = DE.full = HL.full = SP = 0x0000;
  cycles = 0;
  op = 0;

  // setting up the instruction map
  instruction_map[0x31] = FuncDetails(&CPU::IMM16, &CPU::LDSP, 3);
}

void CPU::print_regs() {
  std::cout << "AF: 0x" << std::hex << AF.full << std::endl;
  std::cout << "BC: 0x" << std::hex << BC.full << std::endl;
  std::cout << "DE: 0x" << std::hex << DE.full << std::endl;
  std::cout << "HL: 0x" << std::hex << HL.full << std::endl;
  std::cout << "SP: 0x" << std::hex << SP << std::endl;
  std::cout << "PC: 0x" << std::hex << PC << std::endl;
}

void CPU::clock() {
  if (!cycles) {
    // fetch
    read_ins();

    // decode
    if (instruction_map.find(op) == instruction_map.end()) {
      print_regs();
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

uint8_t CPU::read8(uint16_t addr) { return gba->cpu_read8(addr); }
uint16_t CPU::read16(uint16_t addr) { return gba->cpu_read16(addr); }

void CPU::read_ins() {
  op = read8(PC++); 
  if(PC == 0x0100){
    gba->set_boot_mode(false);
  }
}

void CPU::LDSP() {
  SP = operand;
}

void CPU::XORA() {
  AF.hi = AF.hi ^ AF.hi;
}

void CPU::IMP() {

}

void CPU::IMM16() {
  operand = read16(PC);
  PC += 2;
}

void CPU::IMM8() {
  operand = read8(PC++);
}