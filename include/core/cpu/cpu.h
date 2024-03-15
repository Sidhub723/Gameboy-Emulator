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
  FuncDetails(void (CPU::*ins)(), void (CPU::*addr_mode)(), uint8_t cycles)
      : ins(ins), addr_mode(addr_mode), cycles(cycles) {}
  void (CPU::*ins)();
  void (CPU::*addr_mode)();
  uint8_t cycles;
};

struct CPUState {
  CPUState() {
    PC = 0x0000;
    SP = 0x0000;
    AF.full = BC.full = DE.full = HL.full = 0x0000;
  }

  uint16_t PC;
  uint16_t SP;
  CPURegister AF;
  CPURegister BC;
  CPURegister DE;
  CPURegister HL;
};

class CPU {
public:
  CPU(GB *gb);
  void clock();
  uint8_t read8(uint16_t addr);
  uint16_t read16(uint16_t addr);
  void write8(uint16_t addr, uint8_t data);
  void write16(uint16_t addr, uint16_t data);

  void set_state(CPUState state);
  CPUState get_state();

//SECTION - Utility
public:
  void print_regs();
  void initialize_register_maps();
  void initialize_ins_map();
  void initialize_load_ins();
  void initialize_arithmetic_ins();
  void initialize_push_pop_ins();
  void initialize_ret_ins();
  void initialize_rst_ins();
  void initialize_jmp_ins();
  void initialize_call_ins(); 
  void initialize_jmp_rel_ins();
  void initialize_misc_ins();
  void initialize_rot_ins();

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
  void PFX_SLA_HL(); // Shift Left Arithmetically for HL
  void PFX_RES_R8();
  void PFX_RES_HL();
  void PFX_RLC_R8();
  void PFX_RLC_HL();
  void PFX_RRC_R8();
  void PFX_RRC_HL();
  void PFX_SRA_R8(); //prefix instruction for arithmetic right shift
  void PFX_SRA_HL(); //prefix instruction for arithmetic right shift with HL
  void PFX_SRL_R8(); //prefix instruction for logical right shift
  void PFX_SRL_HL(); //prefix instruction for logical right shift with HL
  void PFX_RL_HL(); // Rotate Left for HL
  void PFX_RL_R8(); // Rotate Left for r8
  void PFX_RR_HL(); // Rotate Right for HL
  void PFX_RR_R8(); // Rotate Right for r8
  void PFX_SWAP_HL();
  void PFX_SWAP_R8();   

//SECTION - Instructions
private:

  // Load instructions
  void LDR8(); // Load into corresponding register from "operand"
  void LDIMM8(); // Load corresponding to the order in register_operands_map
  void LDHL8(); // Load into addr pointed by HL
  void LDA(); // Load into register A
  void LDBC8();
  void LDDE8();
  void LDHLinc();
  void LDHLdec();
  void LDU16addr();
  void LDZ1(); // Zero page write memory : handles 0xE0 & 0xE2
  void LDZ2(); // Zero page write register : handles 0xF0 & 0xF2
  void LD_R16_u16();
  void LD_u16_SP();
  void LD_SP_HL();

  // Arithmetic instructions
  void ADDA(); // add
  void ADCA(); // add with carry
  void SUBA(); // subtract
  void SBCA(); // subtract with carry
  void ANDA(); // AND
  void XORA(); // XOR
  void ORA(); // OR
  void CPA(); // copy
  void INCR8(); // Increment - general case
  void INCHL(); // Increment - (HL) case
  void DECR8(); // Decrement - general case
  void DECHL(); // Decrement - (HL) case
  void INC16();
  void DEC16();
  
  void WRA(); // Write A into Memory
  void ADD_SP(); // Add s8 to SP

  void POP(); // Pop from stack
  void PUSH(); // Push into stack

  // Return
  void RET(); // Return
  void RETI(); // Return from interrupt
  void RET_NZ(); // Return if not zero
  void RET_Z(); // Return if zero
  void RET_NC(); // Return if not carry
  void RET_C(); // Return if carry

  void RST(); // Restart

  // Jump
  void JMP(); // Jump
  void JMP_HL(); // Jump to HL
  void JMP_NZ(); // Jump if not zero
  void JMP_Z(); // Jump if zero
  void JMP_NC(); // Jump if not carry
  void JMP_C(); // Jump if carry

  // Call
  void CALL(); // Call
  void CALL_NZ(); // Call if not zero
  void CALL_Z(); // Call if zero
  void CALL_NC(); // Call if not carry
  void CALL_C(); // Call if carry

  // Jump Relative
  void JR(); // Unoconditional Jump
  void JR_Z(); // Jump if zero
  void JR_NZ(); // Jump if not zero
  void JR_C(); // Jump if carry
  void JR_NC(); // Jump if not carry

  // Misc Instructions
  void NOP(); // No Operation
  void HALT(); // Halt
  void STOP(); // Stop
  void DI(); // Disable Interrupts
  void EI(); // Enable Interrupts
  void DAA(); // Decimal Adjust Accumulator
  void CPL(); // Complement Accumulator
  void SCF(); // Set Carry Flag
  void CCF(); // Complement Carry Flag

  // Rotate Instructions
  void RLCA();
  void RLA();
  void RRCA();
  void RRA();
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
  uint8_t* pfx_register_ptr;
  uint8_t pfx_ins_family_index;
  uint8_t pfx_bit_index;
  uint8_t pfx_register_index;
  uint8_t pfx_rs_family_index;

private:
  uint8_t* register_operands_map[8];
  uint16_t* u16_register_operands_map[4];
  uint16_t* u16_push_pop_register_operands_map[4];
};

#endif