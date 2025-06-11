#include <sstream>
#include <stdexcept>
#include <iostream>
#include "core/cpu/cpu.h"
#include "core/interconnect/gb.h"

uint8_t CPU::read8(uint16_t addr) {
    return gb->read8(addr);
}

uint16_t CPU::read16(uint16_t addr) {
    return gb->read16(addr);
}

void CPU::write8(uint16_t addr, uint8_t data) {
    gb->write8(addr, data);
}

void CPU::write16(uint16_t addr, uint16_t data) {
    gb->write16(addr, data);
}

// Util Functions

void CPU::print_regs() {
  std::cout << std::endl;
  std::cout << "AF: 0x" << std::hex << AF.full << std::endl;
  std::cout << "BC: 0x" << std::hex << BC.full << std::endl;
  std::cout << "DE: 0x" << std::hex << DE.full << std::endl;
  std::cout << "HL: 0x" << std::hex << HL.full << std::endl;
  std::cout << "SP: 0x" << std::hex << SP << std::endl;
  std::cout << "PC: 0x" << std::hex << PC << std::endl;
  std::cout << std::endl;
}
