#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <unordered_map>

class GBA;
class CPU;

union CPURegister {
  struct {
    uint8_t hi; // A
    uint8_t lo; // C
  };
  uint16_t full; // AC
};

struct FuncDetails {
  FuncDetails() {}
  FuncDetails(void (CPU::*addr_mode)(), void (CPU::*ins)(), uint8_t cycles)
      : addr_mode(addr_mode), ins(ins), cycles(cycles) {}
  void (CPU::*addr_mode)();
  void (CPU::*ins)();
  uint8_t cycles;
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

//SECTION - Addressing Modes
private:
  void IMM16(); //Immediate Addressing Mode 16 bits
  void IMM8(); //Immediate Addressing Mode 8 bits
  void IMP(); //Implied Addressing Mode
//!SECTION

//SECTION - Instructions
private:
  void LDSP(); // LOAD INTO SP
  void XORA(); 
//!SECTION

private:
  uint16_t SP; // stack pointer
  uint16_t PC; // program counter
  uint8_t op;  // not riyal
  uint16_t operand; //not riyal - To store the value the instruction operates on

  CPURegister AF;
  CPURegister BC;
  CPURegister DE;
  CPURegister HL;

  std::unordered_map<uint8_t, FuncDetails>
      instruction_map; // maps an opcode to a function(from CPU class) and the
                       // number of cycles it takes

private:
  uint8_t cycles;

private:
  GBA *gba;
};

#endif