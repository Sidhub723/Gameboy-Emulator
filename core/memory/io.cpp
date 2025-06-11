#include <sstream>
#include <stdexcept>
#include <core/memory/io.h>


uint8_t IO::read8(uint16_t offset) {
  return io[offset];
}

void IO::write8(uint16_t offset, uint8_t data) {
  io[offset] = data;
}

uint16_t IO::read16(uint16_t offset) {
  return *((uint16_t*)(io + offset));
}

void IO::write16(uint16_t offset, uint16_t data) {
  *((uint16_t*)(io + offset)) = data;
}
