#include <sstream>
#include <stdexcept>
#include <core/memory/vram.h>


uint8_t VRAM::read8(uint16_t offset) {
  return vram[offset];
}

void VRAM::write8(uint16_t offset, uint8_t data) {
  vram[offset] = data;
}

uint16_t VRAM::read16(uint16_t offset) {
  return *((uint16_t*)(vram + offset));
}

void VRAM::write16(uint16_t offset, uint16_t data) {
  *((uint16_t*)(vram + offset)) = data;
}
