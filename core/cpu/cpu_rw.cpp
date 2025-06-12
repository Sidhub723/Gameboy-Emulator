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
