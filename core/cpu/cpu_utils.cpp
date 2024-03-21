#include <iostream>
#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors
// #include "core/interconnect/gb.h"
#include "core/cpu/cpu.h"

bool CPU::get_flag(uint8_t mask) {
  return AF.lo & ~mask;
}

void CPU::set_flag(uint8_t mask, bool flag_val) {
  if (flag_val) AF.lo = AF.lo | ~mask;
  else AF.lo = AF.lo & mask;
}

void CPU::read_ins() {
  op = read8(PC++); 
  // if(PC == 0x0100){
  //   gb->set_boot_mode(false);
  // }
}

void CPU::call_interrupt(uint16_t addr) {
  ime = false;
  write16(--SP, PC);
  PC = addr;
  cycles = 5;
}

void CPU::initialize_register_maps() {
  // populating the register operands map
  register_operands_map[0b000] = &(BC.hi);
  register_operands_map[0b001] = &(BC.lo);
  register_operands_map[0b010] = &(DE.hi);
  register_operands_map[0b011] = &(DE.lo);
  register_operands_map[0b100] = &(HL.hi);
  register_operands_map[0b101] = &(HL.lo);
  register_operands_map[0b110] = nullptr; //This is the (HL) pointer case
  register_operands_map[0b111] = &(AF.hi);

  // populating the u16 register operands map
  u16_register_operands_map[0b00] = &(BC.full);
  u16_register_operands_map[0b01] = &(DE.full);
  u16_register_operands_map[0b10] = &(HL.full);
  u16_register_operands_map[0b11] = &(SP);

  // populating the u16 push/pop register operands map
  u16_push_pop_register_operands_map[0b00] = &(BC.full);
  u16_push_pop_register_operands_map[0b01] = &(DE.full);
  u16_push_pop_register_operands_map[0b10] = &(HL.full);
  u16_push_pop_register_operands_map[0b11] = &(AF.full);

}

void CPU::initialize_ins_map(){
  //PREFIX Map
  instruction_map[0xcb] = FuncDetails(&CPU::PFX, &CPU::IMP, -1);

  /* Just a random note XD
  Instruction 0x32: LD (HL-) A has been implemented in initialize_load_ins as:
    -- instruction_map[0x32] = FuncDetails(&CPU::LDHLdec, &CPU::LDfromA, 2);

  but could also have been implemented as:
    -- instruction_map[0x32] = FuncDetails(&CPU::WRA, &CPU::HLD, 2);

  So different implementation methods are possible. Maybe calls for some future optimizations!
  */


  // setting up the instruction map
  
  initialize_load_ins();

  initialize_arithmetic_ins();

  initialize_push_pop_ins();

  initialize_ret_ins();

  initialize_rst_ins();

  initialize_jmp_ins();

  initialize_call_ins();

  initialize_jmp_rel_ins();

  initialize_misc_ins();

  initialize_rot_ins();
}

void CPU::initialize_rot_ins() {
  instruction_map[0x07] = FuncDetails(&CPU::RLCA, &CPU::IMP, 1);
  instruction_map[0x17] = FuncDetails(&CPU::RLA, &CPU::IMP, 1);
  instruction_map[0x0F] = FuncDetails(&CPU::RRCA, &CPU::IMP, 1);
  instruction_map[0x1F] = FuncDetails(&CPU::RRA, &CPU::IMP, 1);
}

void CPU::initialize_misc_ins() {
  instruction_map[0x00] = FuncDetails(&CPU::NOP, &CPU::IMP, 1);
  instruction_map[0x10] = FuncDetails(&CPU::STOP, &CPU::IMP, 1);
  instruction_map[0x76] = FuncDetails(&CPU::HALT, &CPU::IMP, 1);
  instruction_map[0xF3] = FuncDetails(&CPU::DI, &CPU::IMP, 1);  
  instruction_map[0xFB] = FuncDetails(&CPU::EI, &CPU::IMP, 1);
  instruction_map[0x27] = FuncDetails(&CPU::DAA, &CPU::IMP, 1);
  instruction_map[0x2F] = FuncDetails(&CPU::CPL, &CPU::IMP, 1);
  instruction_map[0x3F] = FuncDetails(&CPU::CCF, &CPU::IMP, 1);
  instruction_map[0x37] = FuncDetails(&CPU::SCF, &CPU::IMP, 1);
}

void CPU::initialize_call_ins() {
  // Initializing map for CALL ins
  instruction_map[0xCD] = FuncDetails(&CPU::CALL, &CPU::IMM16, 6);

  // Initializing map for CALL cc ins
  instruction_map[0xC4] = FuncDetails(&CPU::CALL_NZ, &CPU::IMM16, (get_flag(Flags::zero) ? 3 : 6));
  instruction_map[0xCC] = FuncDetails(&CPU::CALL_Z, &CPU::IMM16, (get_flag(Flags::zero) ? 6 : 3));
  instruction_map[0xD4] = FuncDetails(&CPU::CALL_NC, &CPU::IMM16, (get_flag(Flags::carry) ? 3 : 6));
  instruction_map[0xDC] = FuncDetails(&CPU::CALL_C, &CPU::IMM16, (get_flag(Flags::carry) ? 6 : 3));
}

// REVIEW : The RESET and RETURN instructions need to be verified
void CPU::initialize_rst_ins() {
  // Initializing map for RST ins
  for(uint16_t op_iter=0xC7; op_iter<=0xFF; op_iter+=0x8){
    instruction_map[(uint8_t)op_iter] = FuncDetails(&CPU::RST, &CPU::IMP, 4);
  }
}

void CPU::initialize_ret_ins() {
  // Initializing map for RET ins
  instruction_map[0xC9] = FuncDetails(&CPU::RET, &CPU::IMP, 4);
  instruction_map[0xD9] = FuncDetails(&CPU::RETI, &CPU::IMP, 4);

  // Initializing map for RET cc ins
  instruction_map[0xC0] = FuncDetails(&CPU::RET_NZ, &CPU::IMP, (get_flag(Flags::zero) ? 2 : 5));
  instruction_map[0xC8] = FuncDetails(&CPU::RET_Z, &CPU::IMP, (get_flag(Flags::zero) ? 5 : 2));
  instruction_map[0xD0] = FuncDetails(&CPU::RET_NC, &CPU::IMP, (get_flag(Flags::carry) ? 2 : 5));
  instruction_map[0xD8] = FuncDetails(&CPU::RET_C, &CPU::IMP, (get_flag(Flags::carry) ? 5 : 2));
}

void CPU::initialize_jmp_ins() {
  // Initializing map for JMP ins
  instruction_map[0xC3] = FuncDetails(&CPU::JMP, &CPU::IMM16, 4);
  instruction_map[0xE9] = FuncDetails(&CPU::JMP_HL, &CPU::IMP, 4);

  instruction_map[0xC2] = FuncDetails(&CPU::JMP_NZ, &CPU::IMM16, (get_flag(Flags::zero) ? 3 : 4));
  instruction_map[0xD2] = FuncDetails(&CPU::JMP_NC, &CPU::IMM16, (get_flag(Flags::carry) ? 3 : 4));
  instruction_map[0xCA] = FuncDetails(&CPU::JMP_Z, &CPU::IMM16, (get_flag(Flags::zero) ? 4 : 3));
  instruction_map[0xDA] = FuncDetails(&CPU::JMP_C, &CPU::IMM16, (get_flag(Flags::carry) ? 4 : 3));
}

// NOTE: THE FOR LOOPS NEED UINT16_T TO AVOID OVERFLOW
void CPU::initialize_push_pop_ins() {
  // Initializing map for PUSH ins
  for(uint16_t op_iter=0xC5; op_iter<=0xF5; op_iter+=0x10){
    instruction_map[(uint8_t)op_iter] = FuncDetails(&CPU::PUSH, &CPU::IMP, 4);
  }

  // Initializing map for POP ins
  for(uint16_t op_iter=0xC1; op_iter<=0xF1; op_iter+=0x10){
    instruction_map[(uint8_t)op_iter] = FuncDetails(&CPU::POP, &CPU::IMP, 3);
  }
}

void CPU::initialize_jmp_rel_ins(){
  // Initializing map for JR ins
  instruction_map[0x18] = FuncDetails(&CPU::JR, &CPU::IMM8, 3);

  // Initializing map for JR conditional ins
  instruction_map[0x20] = FuncDetails(&CPU::JR_NZ, &CPU::IMM8, (get_flag(Flags::zero) ? 2 : 3));
  instruction_map[0x28] = FuncDetails(&CPU::JR_Z, &CPU::IMM8, (get_flag(Flags::zero) ? 3 : 2));
  instruction_map[0x30] = FuncDetails(&CPU::JR_NC, &CPU::IMM8, (get_flag(Flags::carry) ? 2 : 3));
  instruction_map[0x38] = FuncDetails(&CPU::JR_C, &CPU::IMM8, (get_flag(Flags::carry) ? 3 : 2));
}

void CPU::initialize_load_ins() {
  // Initializing map for the 4 rows of LOAD ins
  for(uint16_t op_iter=0x40; op_iter<0x80; op_iter++){
    if(op_iter == 0x76) continue; // HALT ins

    if(op_iter>=0x70 && op_iter<0x80){
      // (HL) case LHS
      instruction_map[(uint8_t)op_iter] = FuncDetails(&CPU::LDHL8, &CPU::LDfromR8, 2);
    }
    else if((op_iter & 0b111) == 0b110){
      // (HL) case RHS
      instruction_map[(uint8_t)op_iter] = FuncDetails(&CPU::LDR8, &CPU::LDfromHL8, 2);
    }
    else {
      instruction_map[(uint8_t)op_iter] = FuncDetails(&CPU::LDR8, &CPU::LDfromR8, 1);
    }
  }

  // Initializing map for +6 & +E LD ins
  for(uint8_t op_iter=0x06; op_iter<=0x3E; op_iter+=0x8){
    uint8_t cyc;
    if((op_iter>>3) == 0b110) cyc=3; // (HL) case
    else cyc=2;

    instruction_map[op_iter] = FuncDetails(&CPU::LDIMM8, &CPU::IMM8, cyc);
  }

  // Initializing map for +2 & +A LD ins
  instruction_map[0x02] = FuncDetails(&CPU::LDBC8, &CPU::LDfromA, 2);
  instruction_map[0x12] = FuncDetails(&CPU::LDDE8, &CPU::LDfromA, 2);
  instruction_map[0x22] = FuncDetails(&CPU::LDHLinc, &CPU::LDfromA, 2);
  instruction_map[0x32] = FuncDetails(&CPU::LDHLdec, &CPU::LDfromA, 2);
  instruction_map[0x0A] = FuncDetails(&CPU::LDA, &CPU::LDfromBC8, 2);
  instruction_map[0x1A] = FuncDetails(&CPU::LDA, &CPU::LDfromDE8, 2);
  instruction_map[0x2A] = FuncDetails(&CPU::LDA, &CPU::HLI, 2);
  instruction_map[0x3A] = FuncDetails(&CPU::LDA, &CPU::HLD, 2);

  // others
  instruction_map[0xEA] = FuncDetails(&CPU::LDU16addr, &CPU::LDfromA, 4);
  instruction_map[0xFA] = FuncDetails(&CPU::LDA, &CPU::LDfromU16addr, 4);
  
  instruction_map[0xE0] = FuncDetails(&CPU::LDZ1, &CPU::IMM8, 3);
  instruction_map[0xE2] = FuncDetails(&CPU::LDZ1, &CPU::LDfromC, 2);
  instruction_map[0xF0] = FuncDetails(&CPU::LDZ2, &CPU::IMM8, 3);
  instruction_map[0xF2] = FuncDetails(&CPU::LDZ2, &CPU::LDfromC, 2);
  
  instruction_map[0x01] = FuncDetails(&CPU::LD_R16_u16, &CPU::IMM16, 3);
  instruction_map[0x11] = FuncDetails(&CPU::LD_R16_u16, &CPU::IMM16, 3);
  instruction_map[0x21] = FuncDetails(&CPU::LD_R16_u16, &CPU::IMM16, 3);
  instruction_map[0x31] = FuncDetails(&CPU::LD_R16_u16, &CPU::IMM16, 3);

  instruction_map[0x08] = FuncDetails(&CPU::LD_u16_SP, &CPU::IMP, 5);
  instruction_map[0xF9] = FuncDetails(&CPU::LD_SP_HL, &CPU::IMP, 2);

}

void CPU::initialize_arithmetic_ins(){
  // Initializing map for the 4 rows of Arithmetic ins
  for(uint8_t op_iter=0x80; op_iter<0xC0; op_iter++){
    uint8_t register_index = op_iter & 0b111;
    switch ((op_iter>>3) & 0b111) { // operation type
      case 0b000:
        // ADD
        if(register_index == 0b110) instruction_map[op_iter] = FuncDetails(&CPU::ADDA, &CPU::LDfromHL8, 2);
        else instruction_map[op_iter] = FuncDetails(&CPU::ADDA, &CPU::LDfromR8, 1);
        break;

      case 0b001:
        // ADC
        if(register_index == 0b110) instruction_map[op_iter] = FuncDetails(&CPU::ADCA, &CPU::LDfromHL8, 2);
        else instruction_map[op_iter] = FuncDetails(&CPU::ADCA, &CPU::LDfromR8, 1);
        break;

      case 0b010:
        // SUB
        if(register_index == 0b110) instruction_map[op_iter] = FuncDetails(&CPU::SUBA, &CPU::LDfromHL8, 2);
        else instruction_map[op_iter] = FuncDetails(&CPU::SUBA, &CPU::LDfromR8, 1);
        break;

      case 0b011:
        // SBC
        if(register_index == 0b110) instruction_map[op_iter] = FuncDetails(&CPU::SBCA, &CPU::LDfromHL8, 2);
        else instruction_map[op_iter] = FuncDetails(&CPU::SBCA, &CPU::LDfromR8, 1);
        break;

      case 0b100:
        // AND
        if(register_index == 0b110) instruction_map[op_iter] = FuncDetails(&CPU::ANDA, &CPU::LDfromHL8, 2);
        else instruction_map[op_iter] = FuncDetails(&CPU::ANDA, &CPU::LDfromR8, 1);
        break;

      case 0b101:
        // XOR
        if(register_index == 0b110) instruction_map[op_iter] = FuncDetails(&CPU::XORA, &CPU::LDfromHL8, 2);
        else instruction_map[op_iter] = FuncDetails(&CPU::XORA, &CPU::LDfromR8, 1);
        break;

      case 0b110:
        // OR
        if(register_index == 0b110) instruction_map[op_iter] = FuncDetails(&CPU::ORA, &CPU::LDfromHL8, 2);
        else instruction_map[op_iter] = FuncDetails(&CPU::ORA, &CPU::LDfromR8, 1);
        break;

      case 0b111:
        // CP
        if(register_index == 0b110) instruction_map[op_iter] = FuncDetails(&CPU::CPA, &CPU::LDfromHL8, 2);
        else instruction_map[op_iter] = FuncDetails(&CPU::CPA, &CPU::LDfromR8, 1);
        break;
    }

    // Initializing INC/DEC ins
    // INC
    for(uint8_t op_iter=0x04; op_iter<=0x3C; op_iter+=0x8){
      if(op_iter>>3 == 0b110)
        instruction_map[op_iter] = FuncDetails(&CPU::INCHL, &CPU::IMP, 3); // (HL) case
      else
        instruction_map[op_iter] = FuncDetails(&CPU::INCR8, &CPU::IMP, 1);
    }
    // DEC
    for(uint8_t op_iter=0x05; op_iter<=0x3D; op_iter+=0x8){
      if(op_iter>>3 == 0b110)
        instruction_map[op_iter] = FuncDetails(&CPU::DECHL, &CPU::IMP, 3); // (HL) case
      else
        instruction_map[op_iter] = FuncDetails(&CPU::DECR8, &CPU::IMP, 1);
    }

    // Initializing R16 INC/DEC ins
    // INC
    instruction_map[0x03] = FuncDetails(&CPU::INC16, &CPU::IMP, 2);
    instruction_map[0x13] = FuncDetails(&CPU::INC16, &CPU::IMP, 2);
    instruction_map[0x23] = FuncDetails(&CPU::INC16, &CPU::IMP, 2);
    instruction_map[0x33] = FuncDetails(&CPU::INC16, &CPU::IMP, 2);
    // DEC
    instruction_map[0x0B] = FuncDetails(&CPU::DEC16, &CPU::IMP, 2);
    instruction_map[0x1B] = FuncDetails(&CPU::DEC16, &CPU::IMP, 2);
    instruction_map[0x2B] = FuncDetails(&CPU::DEC16, &CPU::IMP, 2);
    instruction_map[0x3B] = FuncDetails(&CPU::DEC16, &CPU::IMP, 2);

    // Bottom x6 and xE column
    instruction_map[0xC6] = FuncDetails(&CPU::ADDA, &CPU::IMM8, 2);
    instruction_map[0xCE] = FuncDetails(&CPU::ADCA, &CPU::IMM8, 2);
    instruction_map[0xD6] = FuncDetails(&CPU::SUBA, &CPU::IMM8, 2);
    instruction_map[0xDE] = FuncDetails(&CPU::SBCA, &CPU::IMM8, 2);
    instruction_map[0xE6] = FuncDetails(&CPU::ANDA, &CPU::IMM8, 2);
    instruction_map[0xEE] = FuncDetails(&CPU::XORA, &CPU::IMM8, 2);
    instruction_map[0xF6] = FuncDetails(&CPU::ORA, &CPU::IMM8, 2);
    instruction_map[0xFE] = FuncDetails(&CPU::CPA, &CPU::IMM8, 2);

    // Others
    instruction_map[0xE8] = FuncDetails(&CPU::ADD_SP, &CPU::IMM8, 4);
  }

}

void CPU::print_regs() {
  std::cout << std::endl;
  std::cout << "AF: 0x" << std::hex << AF.full << std::endl;
  std::cout << "BC: 0x" << std::hex << BC.full << std::endl;
  std::cout << "DE: 0x" << std::hex << DE.full << std::endl;
  std::cout << "HL: 0x" << std::hex << HL.full << std::endl;
  std::cout << "SP: 0x" << std::hex << SP << std::endl;
  std::cout << "PC: 0x" << std::hex << PC << std::endl << std::endl;
}

void CPU::set_state(CPUState state) {
  AF.full = state.AF.full;
  BC.full = state.BC.full;
  DE.full = state.DE.full;
  HL.full = state.HL.full;
  SP = state.SP;
  PC = state.PC;
}

CPUState CPU::get_state() {
  CPUState state;
  state.AF.full = AF.full;
  state.BC.full = BC.full;
  state.DE.full = DE.full;
  state.HL.full = HL.full;
  state.SP = SP;
  state.PC = PC;
  return state;
}