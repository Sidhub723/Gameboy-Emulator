#ifndef VRAM_H
#define VRAM_H

#include <stdint.h>
#include <string>

class VRAM;


class VRAM {
public:
  VRAM() {};
  ~VRAM() {};
  uint8_t read8(uint16_t addr);
  void write8(uint16_t addr, uint8_t data);
  uint16_t read16(uint16_t addr);
  void write16(uint16_t addr, uint16_t data);

private:
  uint8_t vram[0x2000] = {0xcd};
};

#endif