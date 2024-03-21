#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors
#include <core\memory\io.h>


uint8_t IO::read8(uint16_t addr) {
  return io[addr];
}

void IO::write8(uint16_t addr, uint8_t data) {
  io[addr] = data;
}

uint16_t IO::read16(uint16_t addr) {
  return *((uint16_t*)(io + addr));
}

void IO::write16(uint16_t addr, uint16_t data) {
  *((uint16_t*)(io + addr)) = data;
}