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
  instruction_map[0x31] = std::make_pair(&CPU::LDSP, 3);
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
    (this->*(instruction_map[op].first))();
    cycles = instruction_map[op].second;
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
  uint16_t imm = read16(PC);
  PC += 2;
  SP = imm;
}

void CPU::XOR() {
  AF.hi = AF.hi
}