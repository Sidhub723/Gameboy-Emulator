#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <unordered_map>
#include <utility>

class GBA;

class CPU {
public:
  CPU(GBA *gba);
  void clock();
  uint8_t read8(uint16_t addr);

private:
  void read_ins();

private:
  uint16_t SP; // stack pointer
  uint16_t PC; // program counter
  uint8_t op;  // not riyal

  uint16_t AF;
  uint16_t BC;
  uint16_t DE;
  uint16_t HL;

  std::unordered_map<uint8_t, std::pair<void (CPU::*)(), uint8_t>>
      instruction_map; // maps an opcode to a function(from CPU class) and the
                       // number of cycles it takes

private:
  uint8_t cycles;

private:
  GBA *gba;
};

#endif