#include <sstream>
#include <stdexcept>
#include "core/cpu/cpu.h"

// SECTION - PREFIX INSTRUCTIONS

// Main Loop
void CPU::PFX()
{
  read_ins();

  pfx_ins_family_index = (op & 0b11000000) >> 6;
  pfx_bit_index = (op & 0b00111000) >> 3;
  pfx_register_index = (op & 0b00000111);
  pfx_register_ptr = register_operands_map[pfx_register_index];

  if (pfx_ins_family_index == 0b00) {
    // Rotate/Shift family
    pfx_rs_family_index = (op & 0b00111000) >> 3; // equivalent to pfx_bit_index

    if (pfx_rs_family_index == 0b100) {
      // SLA
      if (pfx_register_index == 0b110) { // 0b110 corresponds to (HL) operand
        HL_Wrapper(PFX_SLA);
        cycles = (3 + 1);
      }
      else {
        R8_Wrapper(PFX_SLA);
        cycles = (1 + 1);
      }
    }

    if (pfx_rs_family_index == 0b000) {
      // RLC
      if (pfx_register_index == 0b110) {
        HL_Wrapper(PFX_RLC);
        cycles = (3 + 1);
      }
      else {
        R8_Wrapper(PFX_RLC);
        cycles = (1 + 1);
      }
    }

    if (pfx_bit_index == 0b101) { // SRA
      // SRA
      if (pfx_register_index == 0b110) {
        HL_Wrapper(PFX_SRA);
        cycles = (3 + 1);
      }
      else {
        R8_Wrapper(PFX_SRA);
        cycles = (1 + 1);
      }
    }

    if (pfx_rs_family_index == 0b001) {
      // RRC
      if (pfx_register_index == 0b110) {
        HL_Wrapper(PFX_RRC);
        cycles = (3 + 1);
      }
      else {
        R8_Wrapper(PFX_RRC);
        cycles = (1 + 1);
      }
    }

    if (pfx_bit_index == 0b111) { // SRL
      // SRL
      if (pfx_register_index == 0b110) {
        HL_Wrapper(PFX_SRL);
        cycles = (3 + 1);
      }
      else {
        R8_Wrapper(PFX_SRL);
        cycles = (1 + 1);
      }
    }

    if (pfx_rs_family_index == 0b010) {
      // RL
      if (pfx_register_index == 0b110) { // 0b110 corresponds to (HL) operand
        HL_Wrapper(PFX_RL);
        cycles = (3 + 1);
      }
      else {
        R8_Wrapper(PFX_RL);
        cycles = (1 + 1);
      }
    }

    if (pfx_rs_family_index == 0b011) {
      // RR
      if (pfx_register_index == 0b110) { // 0b110 corresponds to (HL) operand
        HL_Wrapper(PFX_RR);
        cycles = (3 + 1);
      }
      else {
        R8_Wrapper(PFX_RR);
        cycles = (1 + 1);
      }
    }

    if (pfx_rs_family_index == 0b110) {
      // SWAP
      if (pfx_register_index == 0b110) {
        HL_Wrapper(PFX_SWAP);
        cycles = (3 + 1);
      }
      else {
        R8_Wrapper(PFX_SWAP);
        cycles = (1 + 1);
      }
    }
  }
  else if (pfx_ins_family_index == 0b01) {
    // BIT family
    if (pfx_register_index == 0b110) { // 0b110 corresponds to (HL) operand
      HL_Wrapper(PFX_BIT);
      cycles = (2 + 1);
    }
    else {
      R8_Wrapper(PFX_BIT);
      cycles = (1 + 1);
    }
  }
  else if (pfx_ins_family_index == 0b10) {
    // RES family
    if (pfx_register_index == 0b110) { // 0b110 corresponds to (HL) operand
      HL_Wrapper(PFX_RES);
      cycles = (3 + 1);
    }
    else {
      R8_Wrapper(PFX_RES);
      cycles = (1 + 1);
    }
  }
  else if (pfx_ins_family_index == 0b11) {
    // SET family
    if (pfx_register_index == 0b110) { // 0b110 corresponds to (HL) operand
      HL_Wrapper(PFX_SET);
      cycles = (3 + 1);
    }
    else {
      R8_Wrapper(PFX_SET);
      cycles = (1 + 1);
    }
  }
}

// Wrapper functions for R8 & HL cases
void CPU::R8_Wrapper(void (CPU::*ins)()) {
  operand = *pfx_register_ptr;
  (this->*ins)();
  *pfx_register_ptr = operand;
}

void CPU::HL_Wrapper(void (CPU::*ins)()) {
  operand = read8(HL.full);
  (this->*ins)();
  write8(HL.full, operand);
}

// Instructions for Prefix Family
void CPU::PFX_SET()
{
  operand |= (1 << pfx_bit_index);
}

void CPU::PFX_BIT()
{
  set_flag(Flags::zero, !(operand & (1 << pfx_bit_index)));
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 1);
}

void CPU::PFX_RES()
{
  operand &= ~(1 << pfx_bit_index);
}

void CPU::PFX_SLA()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, (operand & 0b10000000));
  operand <<= 1;
  set_flag(Flags::zero, operand == 0);
}

void CPU::PFX_RL()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  uint8_t temp = get_flag(Flags::carry);
  set_flag(Flags::carry, ((operand) & (0b10000000)));
  (operand) <<= 1;
  operand |= temp; // Based on the previous carry flag
  set_flag(Flags::zero, !(operand));
}

void CPU::PFX_RR()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  uint8_t temp = get_flag(Flags::carry);
  set_flag(Flags::carry, ((operand) & (0b00000001)));
  (operand) >>= 1;
  operand |= (temp << 7); // Based on the previous carry flag
  set_flag(Flags::zero, !(operand));
}

void CPU::PFX_SWAP()
{
  uint8_t upnibble = operand >> 4;
  operand = operand << 4;
  operand = operand | upnibble;

  set_flag(Flags::zero, !(operand));
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, 0);
}

void CPU::PFX_SRA()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, (operand & 0b00000001));
  operand = (operand >> 1) | (operand & 0b10000000); 
  set_flag(Flags::zero, operand == 0);
}

void CPU::PFX_SRL()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, (operand & 0b00000001));
  operand = operand >> 1;
  set_flag(Flags::zero, operand == 0);
}

void CPU::PFX_RLC()
{
  uint8_t bit_7 = (operand) >> 7;
  operand <<= 1;
  operand |= bit_7;

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::zero, operand == 0);
  set_flag(Flags::carry, bit_7);
}

void CPU::PFX_RRC()
{
  uint8_t bit_0 = (operand) & 1;
  operand >>= 1;
  operand |= (bit_0 << 7);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::zero, operand == 0);
  set_flag(Flags::carry, bit_0);
}
