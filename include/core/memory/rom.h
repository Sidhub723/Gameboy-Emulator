#ifndef ROM_H
#define ROM_H

#include <stdint.h>
#include <string>

class ROM;


class ROM {
public:
  ROM(std::string file_path);
  ~ROM() {};
  uint8_t read8(uint16_t addr);
  uint16_t read16(uint16_t addr);

private:
  uint8_t *rom;
};

#endif