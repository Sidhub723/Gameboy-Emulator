#include <sstream>
#include <stdexcept>
#include <core/memory/hram.h>


uint8_t HRAM::read8(uint16_t offset) {
  return hram[offset];
}

void HRAM::write8(uint16_t offset, uint8_t data) {
  hram[offset] = data;
}

uint16_t HRAM::read16(uint16_t offset) {
  return *((uint16_t*)(hram + offset));
}

void HRAM::write16(uint16_t offset, uint16_t data) {
  *((uint16_t*)(hram + offset)) = data;
}
