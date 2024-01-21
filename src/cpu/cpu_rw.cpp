#include "../../include/CPU.h"
#include "../../include/GBA.h"


#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

uint8_t CPU::read8(uint16_t addr) { return gba->cpu_read8(addr); }
uint16_t CPU::read16(uint16_t addr) { return gba->cpu_read16(addr); }

void CPU::write8(uint16_t addr, uint8_t data) {gba->cpu_write8(addr, data);}
void CPU::write16(uint16_t addr, uint16_t data) {gba->cpu_write16(addr, data);}