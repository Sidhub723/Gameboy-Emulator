#include "../../include/CPU.h"


#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

//SECTION - PREFIX INSTRUCTIONS

void CPU::PFX()
{
  read_ins();

  pfx_ins_family_index = (op & 0b11000000) >> 6;
  pfx_bit_index = (op & 0b00111000) >> 3;
  pfx_register_index = (op & 0b00000111);
  pfx_register_ptr = register_operands_map[pfx_register_index];

  if (pfx_ins_family_index == 0b00){
    // Rotate/Shift family
    pfx_rs_family_index = (op & 0b00111000) >> 3; // equivalent to pfx_bit_index
    
    if (pfx_rs_family_index == 0b100){
      if(pfx_register_index == 0b110){ //0b110 corresponds to (HL) operand
        PFX_SLA_HL();
        cycles = (3+1);
      }
      else{
        PFX_SLA_R8();
        cycles = (1+1);
      }
    }

    if (pfx_rs_family_index == 0b000){
      //RLC
      if(pfx_register_index == 0b110){
        PFX_RLC_HL();
        cycles = (3+1);
      }
      else{
        PFX_RLC_R8();
        cycles = (1+1);
      }
    }

    if (pfx_bit_index == 0b101){ // SRA
      if(pfx_register_index == 0b110){
        PFX_SRA_HL();
        cycles = (3+1);
      }
      else{
        PFX_SRA_R8();
        cycles = (1+1);
      }
    }

    if (pfx_rs_family_index == 0b001){
      //RRC
      if(pfx_register_index == 0b110){
        PFX_RRC_HL();
        cycles = (3+1);
      }
      else{
        PFX_RRC_R8();
        cycles = (1+1);
      }
    }
    
    if (pfx_bit_index == 0b111){ // SRL
      if(pfx_register_index == 0b110){
        PFX_SRL_HL();
        cycles = (3+1);
      }
      else{
        PFX_SRL_R8();
        cycles = (1+1);
      }
    }

    if(pfx_rs_family_index == 0b010) { //rl instruction
      if(pfx_register_index == 0b110){ //0b110 corresponds to (HL) operand
        PFX_RL_HL();
        cycles = (3+1);
      }
      else{
        PFX_RL_R8();
        cycles = (1+1);
      }
    }

    if(pfx_rs_family_index == 0b011) { //rr instruction
      if(pfx_register_index == 0b110){ //0b110 corresponds to (HL) operand
        PFX_RR_HL();
        cycles = (3+1);
      }
      else{
        PFX_RR_R8();
        cycles = (1+1);
      }
    }

    if (pfx_rs_family_index == 0b110){ // SWAP instruction
      if(pfx_register_index == 0b110){ 
        PFX_SWAP_HL();
        cycles = (3+1);
      }
      else{
        PFX_SWAP_R8();
        cycles=(1+1);
      }
    }

  }
  else if (pfx_ins_family_index == 0b01){
    // BIT family
    if (pfx_register_index == 0b110){ // 0b110 corresponds to (HL) operand
      PFX_BIT_HL();
      cycles = (2+1);
    }
    else{
      PFX_BIT_R8();
      cycles = (1+1);
    }
  }
  else if (pfx_ins_family_index == 0b10){
    // RES family
    if (pfx_register_index == 0b110){ // 0b110 corresponds to (HL) operand
      PFX_RES_HL();
      cycles = (3+1);
    }
    else{
      PFX_RES_R8();
      cycles = (1+1);
    }
  }
  else if (pfx_ins_family_index == 0b11){
    // SET family
    if (pfx_register_index == 0b110){ // 0b110 corresponds to (HL) operand
      PFX_SET_HL();
      cycles = (3+1);
    }
    else{
      PFX_SET_R8();
      cycles = (1+1);
    }
  }

}

void CPU::PFX_SET_HL()
{
    operand = read8(HL.full);
    operand_addr = HL.full;
    operand |= (1 << pfx_bit_index);
    write8(operand_addr, operand);
}
 
void CPU::PFX_SET_R8()
{
  *pfx_register_ptr |= (1 << pfx_bit_index);
}


void CPU::PFX_BIT_HL()
{ //here we will use the CPU::HL Register
    operand_addr=HL.full;
    operand = read8(HL.full);
    set_flag(Flags::zero, ~( operand & (1 << pfx_bit_index) ));
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 1);
}

void CPU::PFX_BIT_R8()
{
    set_flag(Flags::zero, ~( *pfx_register_ptr & (1 << pfx_bit_index) ));
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 1);
}


void CPU::PFX_RES_HL()
{
  operand = read8(HL.full);
  operand_addr = HL.full;
  operand &= ~(1 << pfx_bit_index);
  write8(operand_addr, operand);
}


void CPU::PFX_RES_R8()
{
  *pfx_register_ptr &= ~(1 << pfx_bit_index);
}

void CPU::PFX_SLA_R8()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, ((*pfx_register_ptr) & 0b10000000));
  *pfx_register_ptr = (*pfx_register_ptr<<1);
  if(*pfx_register_ptr==0)
    set_flag(Flags::zero, 1);
}

void CPU::PFX_SLA_HL()
{
  //here we will use the CPU::HL register
  operand_addr=HL.full;
  operand = read8(operand_addr);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::carry, (operand & 0b10000000));
  operand <<= 1;
  if(operand==0)
    set_flag(Flags::zero, 1);

  write8(operand_addr,operand);
  
}

void CPU::PFX_RL_HL() {
  operand_addr = HL.full;
  operand = read8(operand_addr);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  uint8_t temp = get_flag(Flags::carry);
  set_flag(Flags::carry,((operand)&(0b10000000)));
  (operand) <<= 1;
  if(temp) //checking if previous carry flag was set or not
    (operand) |= (0b00000001);
  if((operand) == 0)
    set_flag(Flags::zero,1);

  write8(operand_addr,operand);

}

void CPU::PFX_RL_R8() {
  operand = (*pfx_register_ptr);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  uint8_t temp = get_flag(Flags::carry);
  set_flag(Flags::carry,((operand)&(0b10000000)));
  (operand) <<= 1;
  if(temp) //checking if previous carry flag was set or not
    (operand) |= (0b00000001);
  if((operand) == 0)
    set_flag(Flags::zero,1);

  (*pfx_register_ptr) = operand;
}

void CPU::PFX_RR_HL() {
  operand_addr = HL.full;
  operand = read8(operand_addr);
  
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  uint8_t temp = get_flag(Flags::carry);
  set_flag(Flags::carry,((operand)&(0b00000001)));
  (operand) >>= 1;
  if(temp) //checking if previous carry flag was set or not
    (operand) |= (0b10000000);
  if((operand) == 0)
    set_flag(Flags::zero,1);

  write8(operand_addr, operand);


}

void CPU::PFX_RR_R8() {
  operand = (*pfx_register_ptr);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  uint8_t temp = get_flag(Flags::carry);
  set_flag(Flags::carry,((operand)&(0b00000001)));
  (operand) >>= 1;
  if(temp) //checking if previous carry flag was set or not
    (operand) |= (0b10000000);
  if((operand) == 0)
    set_flag(Flags::zero,1);

  (*pfx_register_ptr) = operand;
  
}

void CPU::PFX_SWAP_R8()
{    
    uint8_t upnibble = (*pfx_register_ptr)>>4;
    *pfx_register_ptr = (*pfx_register_ptr)<<4;
    *pfx_register_ptr = *pfx_register_ptr|upnibble;
    set_flag(Flags::zero, !(*pfx_register_ptr));
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 0);
    set_flag(Flags::carry, 0);
}

void CPU::PFX_SWAP_HL(){
    operand_addr=HL.full;
    operand = read8(HL.full); 
    uint8_t upnibble=operand>>4;
    operand=operand<<4;
    operand=operand|upnibble;
    write8(operand_addr, operand);

    set_flag(Flags::zero, !(operand));
    set_flag(Flags::neg, 0); 
    set_flag(Flags::half_carry, 0);
    set_flag(Flags::carry, 0);
}

void CPU::PFX_SRA_HL()
{
  operand = read8(HL.full);
  operand_addr = HL.full;

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if(operand & 0b00000001){
    set_flag(Flags::carry, 1);
  }
  else
    set_flag(Flags::carry, 0);
  
  if(operand & 0b10000000){
    operand = (operand>>1) | 0b10000000; // After right shifting by 1b, preserve the MSB
  }
  else{
    operand = (operand>>1); // If MSB is zero, just do right shift
  }
  if(operand==0){
    set_flag(Flags::zero, 1);
  }

  write8(operand_addr, operand);  
}


void CPU::PFX_SRA_R8()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if(*pfx_register_ptr & 0b00000001){
    set_flag(Flags::carry, 1);
  }
  else{
    set_flag(Flags::carry, 0);
  }
  if(*pfx_register_ptr & 0b10000000){
    *pfx_register_ptr = (*pfx_register_ptr>>1) | 0b10000000; // After right shifting by 1b, preserve the MSB
  }
  else{
    *pfx_register_ptr = (*pfx_register_ptr>>1); // If MSB is zero, just do right shift
  }
  if(*pfx_register_ptr==0){
    set_flag(Flags::zero, 1);
  }
}


void CPU::PFX_SRL_HL()
{
  operand = read8(HL.full);
  operand_addr = HL.full;

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if(operand & 0b00000001){
    set_flag(Flags::carry, 1);
  }
  else
    set_flag(Flags::carry, 0);
  
  operand = operand >> 1;

  if(operand==0){
    set_flag(Flags::zero, 1);
  }

  write8(operand_addr, operand);
}


void CPU::PFX_SRL_R8()
{
  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  if(*pfx_register_ptr & 0b00000001){
    set_flag(Flags::carry, 1);
  }
  else
    set_flag(Flags::carry, 0);
  
  *pfx_register_ptr = (*pfx_register_ptr >> 1);

  if(*pfx_register_ptr==0){
    set_flag(Flags::zero, 1);
  }
}

void CPU::PFX_RLC_R8()
{
  uint8_t bit_7 = (*pfx_register_ptr) >> 7;
  *pfx_register_ptr <<= 1;
  *pfx_register_ptr |= bit_7;

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::zero, *pfx_register_ptr == 0);
  set_flag(Flags::carry, bit_7);
}

void CPU::PFX_RLC_HL()
{
  operand = read8(HL.full);
  operand_addr = HL.full;
  uint8_t bit_7 = (operand) >> 7;
  operand <<= 1;
  operand |= bit_7;
  write8(operand_addr, operand);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::zero, operand == 0);
  set_flag(Flags::carry, bit_7);
}

void CPU::PFX_RRC_R8()
{
  uint8_t bit_0 = (*pfx_register_ptr) & 1;
  *pfx_register_ptr >>= 1;
  *pfx_register_ptr |= (bit_0 << 7);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::zero, *pfx_register_ptr == 0);
  set_flag(Flags::carry, bit_0);
}

void CPU::PFX_RRC_HL()
{
  operand = read8(HL.full);
  operand_addr = HL.full;
  uint8_t bit_0 = (operand) & 1;
  operand >>= 1;
  operand |= (bit_0 << 7);
  write8(operand_addr, operand);

  set_flag(Flags::neg, 0);
  set_flag(Flags::half_carry, 0);
  set_flag(Flags::zero, operand == 0);
  set_flag(Flags::carry, bit_0);
}
