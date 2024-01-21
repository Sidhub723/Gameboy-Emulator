#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <unordered_map>

class GBA;
class CPU;

union CPURegister {
  struct {
    uint8_t lo; // C
    uint8_t hi; // A
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
  void write8(uint16_t addr, uint8_t data);
  void write16(uint16_t addr, uint16_t data);

//SECTION - Utility
public:
  void print_regs();

private:
  void read_ins();
  
  enum Flags {
    zero = 0b01111111,
    neg = 0b10111111,
    half_carry = 0b11011111,
    carry = 0b11101111
  };

  bool get_flag(uint8_t mask);
  void set_flag(uint8_t mask, bool flag_val);
//!SECTION

//SECTION - Addressing Modes
private:
  void IMM16(); //Immediate Addressing Mode 16 bits
  void IMM8(); //Immediate Addressing Mode 8 bits
  void IMP(); //Implied Addressing Mode
  void HLI(); //Auto Increment Mode
  void HLD(); // Auto Decrement Mode

  void PFXH();
  void PFXA();
  void PFXB();
  void PFXC();
  void PFXD();
  void PFXL();
  void PFXE();
  void PFXHL_P();

//!SECTION

//SECTION - Instructions
private:
  void LDSP(); // LOAD INTO SP
  void XORA(); // XOR
  void LDHL(); // Load into Register HL
  void WRA(); // Write A into Memory

  void PFX(); //PREFIX instruction

  void BIT7(); //Check if the 7th bit of the H register is set
  void BIT6(); //Check if the 6th bit of the H register is set
  void BIT5(); //Check if the 5th bit of the H register is set
  void BIT4(); //Check if the 4th bit of the H register is set
  void BIT3(); //Check if the 3th bit of the H register is set
  void BIT2(); //Check if the 2th bit of the H register is set
  void BIT1(); //Check if the 1th bit of the H register is set
  void BIT0(); //Check if the 0th bit of the H register is set


//!SECTION

private:
  uint16_t SP; // stack pointer
  uint16_t PC; // program counter
  uint8_t op;  // not riyal
  uint16_t operand; //not riyal - To store the value the instruction operates on
  uint16_t operand_addr; //not riyal - To store address of the val

  CPURegister AF;
  CPURegister BC;
  CPURegister DE;
  CPURegister HL;

  std::unordered_map<uint8_t, FuncDetails>
      instruction_map; // maps an opcode to a function(from CPU class) and the
                       // number of cycles it takes

  std::unordered_map<uint8_t, FuncDetails>
      prefix_map;

private:
  uint8_t cycles;

private:
  GBA *gba;

private:
  FuncDetails pfx_reg_array[8];
  FuncDetails pfx_bit_array[8];
};

#endif