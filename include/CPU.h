#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <unordered_map>

class GB;
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
  CPU(GB *gb);
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

//!SECTION Prefix Instructions
private:
  void PFX();
  void PFX_SET_R8();
  void PFX_SET_HL();
  void PFX_BIT_HL(); //prefix instruction for BIT Instructions with HL
  void PFX_BIT_R8(); //prefix instruction for BIT Instructions 
  void PFX_SLA_R8(); // Shift Left Arithmetically for r8
  void PFX_SLA_HL(); //Shift Left Arithmetically for HL
  void PFX_RES_R8();
  void PFX_RES_HL();

//SECTION - Instructions
private:
  void LDSP(); // LOAD INTO SP
  void XORA(); // XOR
  void LDHL(); // Load into Register HL
  void WRA(); // Write A into Memory

//!SECTION

private:
  uint16_t SP; // stack pointer
  uint16_t PC; // program counter
  uint8_t op;  // not riyal
  uint16_t operand; //not riyal - To store the value the instruction operates on
  uint16_t operand_addr; //not riyal - To store address of the val
  uint8_t* register_operands_map[8];

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
  GB *gb;

private:
  //Add the other members you create here
  uint8_t* pfx_register_ptr;
  uint8_t pfx_ins_family_index;
  uint8_t pfx_bit_index;
  uint8_t pfx_register_index;
  uint8_t pfx_rs_family_index;

private:
  bool LoadInsHelper(); // executes and returns true if load ins is found
  bool ArithmeticInsHelper(); // executes and returns true if arithmetic ins is found
  uint8_t* temp_u8_ptr;
  uint8_t* temp_u8_ptr_2;
};

#endif