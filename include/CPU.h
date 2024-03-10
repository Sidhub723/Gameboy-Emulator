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
  void initialize_ins_map();
  void initialize_load_ins();
  void initialize_arith_ins();

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
  void LDfromR8(); // Loads corresponding register value into "operand"
  void LDfromHL8(); // Loads value at (HL) into "operand"
  void LDfromA(); // Loads from A - special case
  void LDfromC();
  void LDfromBC8();
  void LDfromDE8();
  void LDfromU16addr();

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

  // Load instructions
  void LDSP(); // Load INTO SP
  void LDHL16(); // Load into Register HL
  void LDR8(); // Load into corresponding register from "operand"
  void LDB(); // Load into register B
  void LDC(); // Load into register C
  void LDD(); // Load into register D
  void LDE(); // Load into register E
  void LDH(); // Load into register H
  void LDL(); // Load into register L
  void LDHL8(); // Load into addr pointed by HL
  void LDA(); // Load into register A
  void LDBC8();
  void LDDE8();
  void LDHLinc();
  void LDHLdec();
  void LDU16addr();
  void LDZ1(); // Zero page write memory : handles 0xE0 & 0xE2
  void LDZ2(); // Zero page write register : handles 0xF0 & 0xF2

  void XORA(); // XOR
  void WRA(); // Write A into Memory

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

private:
  uint8_t cycles;

private:
  GB *gb;

private:
  //Add the other members you create here
  uint8_t* pfx_register_operands_map[8];
  uint8_t* pfx_register_ptr;
  uint8_t pfx_ins_family_index;
  uint8_t pfx_bit_index;
  uint8_t pfx_register_index;
  uint8_t pfx_rs_family_index;
};

#endif