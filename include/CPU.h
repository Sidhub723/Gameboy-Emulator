#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <unordered_map>
#include <utility>

class GBA;

union CPURegister {
  struct {
    uint8_t hi; // A
    uint8_t lo; // C
  };
  uint16_t full; // AC
};

class CPU {
public:
  CPU(GBA *gba);
  void clock();
  uint8_t read8(uint16_t addr);
  uint16_t read16(uint16_t addr);

//SECTION - Utility
private:
  void read_ins();
  void print_regs();
//!SECTION

//SECTION - Instructions
private:
  void LDSP(); // LOAD INTO SP
  void XOR(); 
//!SECTION

private:
  uint16_t SP; // stack pointer
  uint16_t PC; // program counter
  uint8_t op;  // not riyal

  CPURegister AF;
  CPURegister BC;
  CPURegister DE;
  CPURegister HL;

  std::unordered_map<uint8_t, std::pair<void (CPU::*)(), uint8_t>>
      instruction_map; // maps an opcode to a function(from CPU class) and the
                       // number of cycles it takes

private:
  uint8_t cycles;

private:
  GBA *gba;
};

#endif