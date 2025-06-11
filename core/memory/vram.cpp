#include <sstream>
#include <stdexcept>
#include <core/memory/vram.h>


uint8_t VRAM::read8(uint16_t addr) {
  return vram[addr];
}

void VRAM::write8(uint16_t addr, uint8_t data) {
  vram[addr] = data;
}

uint16_t VRAM::read16(uint16_t addr) {
  return *((uint16_t*)(vram + addr));
}

void VRAM::write16(uint16_t addr, uint16_t data) {
  *((uint16_t*)(vram + addr)) = data;
}
