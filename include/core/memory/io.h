#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <string>


class IO {
public:
  IO() {};
  ~IO() {};
  uint8_t read8(uint16_t offset);
  void write8(uint16_t offset, uint8_t data);
  uint16_t read16(uint16_t offset);
  void write16(uint16_t offset, uint16_t data);

private:
  uint8_t io[0x80] = {0x00};
};

#endif
