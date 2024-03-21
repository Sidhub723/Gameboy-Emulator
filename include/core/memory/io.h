#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <string>

class IO;


class IO {
public:
  IO() {};
  ~IO() {};
  uint8_t read8(uint16_t addr);
  void write8(uint16_t addr, uint8_t data);
  uint16_t read16(uint16_t addr);
  void write16(uint16_t addr, uint16_t data);

private:
  uint8_t io[0x80] = {0x00};
};

#endif