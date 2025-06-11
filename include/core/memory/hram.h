#ifndef HRAM_H
#define HRAM_H

#include <stdint.h>
#include <string>

#define BOOT_ROM_DISABLE 0xFF50


class HRAM {
public:
  HRAM() { hram = new uint8_t[0x7F]; } // 127 bytes
  ~HRAM() {}
  uint8_t read8(uint16_t offset);
  void write8(uint16_t offset, uint8_t data);
  uint16_t read16(uint16_t offset);
  void write16(uint16_t offset, uint16_t data);

private:
  uint8_t* hram;
};

#endif
