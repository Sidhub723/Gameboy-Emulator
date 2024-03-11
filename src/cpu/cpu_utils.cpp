#include "../../include/GB.h"
#include "../../include/CPU.h"

#include <iostream>
#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

bool CPU::get_flag(uint8_t mask) {
  return AF.lo & ~mask;
}

void CPU::set_flag(uint8_t mask, bool flag_val) {
  if (flag_val) AF.lo = AF.lo | ~mask;
  else AF.lo = AF.lo & mask;
}

void CPU::read_ins() {
  op = read8(PC++); 
  if(PC == 0x0100){
    gb->set_boot_mode(false);
  }
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
}

void CPU::initialize_ins_map(){
  //PREFIX Map
  instruction_map[0xcb] = FuncDetails(&CPU::IMP, &CPU::PFX, -1);

  // setting up the instruction map
  instruction_map[0x21] = FuncDetails(&CPU::IMM16, &CPU::LDHL16, 3);
  instruction_map[0x31] = FuncDetails(&CPU::IMM16, &CPU::LDSP, 3);
  instruction_map[0xaf] = FuncDetails(&CPU::IMP, &CPU::XORA, 1);
  instruction_map[0x32] = FuncDetails(&CPU::HLD, &CPU::WRA, 2);

  initialize_load_ins();

  initialize_arithmetic_ins();
}


void CPU::initialize_load_ins() {
  // Initializing map for the 4 rows of LOAD ins
  for(uint8_t op_iter=0x40; op_iter<0x80; op_iter++){
    if(op_iter=0x76) continue; // HALT ins

    if(op_iter>=0x70 && op_iter<0x80){
      // (HL) case LHS
      instruction_map[op_iter] = FuncDetails(&CPU::LDHL8, &CPU::LDfromR8, 2);
    }
    else if(op_iter & 0b111 == 0b110){
      // (HL) case RHS
      instruction_map[op_iter] = FuncDetails(&CPU::LDR8, &CPU::LDfromHL8, 2);
    }
    else {
      instruction_map[op_iter] = FuncDetails(&CPU::LDR8, &CPU::LDfromR8, 1);
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