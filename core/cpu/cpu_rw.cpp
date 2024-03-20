#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors
#include "core/cpu/cpu.h"
#include "core/interconnect/gb.h"

uint8_t CPU::read8(uint16_t addr) { return gb->cpu_read8(addr); }
uint16_t CPU::read16(uint16_t addr) { return gb->cpu_read16(addr); }

void CPU::write8(uint16_t addr, uint8_t data) {gb->cpu_write8(addr, data);}
void CPU::write16(uint16_t addr, uint16_t data) {gb->cpu_write16(addr, data);}

void CPU::DI() {ime = false;}
void CPU::EI() {ime = true;}