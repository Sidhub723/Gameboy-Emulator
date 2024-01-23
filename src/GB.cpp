#include "../include/GB.h"
#include "../include/Boot.h"
#include "../include/CPU.h"
#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

GB::GB(std::string boot_file_path) {
  boot_mode_enabled = true;
  boot = new Boot(boot_file_path);
  cpu = new CPU(this);
}
GB::~GB() {
  delete cpu;
  delete boot;
}
uint8_t GB::cpu_read8(uint16_t addr) {

  if (boot_mode_enabled && boot_range.in_range(addr)) {
    return boot->read8(boot_range.offset_of(addr));
  }
  else if (vram_range.in_range(addr)){
    return VRAM[vram_range.offset_of(addr)];
  }

  std::stringstream ss;
  ss << "Unmapped address for cpu_read8: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

uint16_t GB::cpu_read16(uint16_t addr) {

  if (boot_mode_enabled && boot_range.in_range(addr)) {
    return boot->read16(boot_range.offset_of(addr));
  }

  std::stringstream ss;
  ss << "Unmapped address for cpu_read16: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

void GB::cpu_write8(uint16_t addr, uint8_t data) {
  if(vram_range.in_range(addr)){
    VRAM[vram_range.offset_of(addr)] = data;
    return;
  }
  
  std::stringstream ss;
  ss << "Unmapped address for cpu_write8: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

void GB::cpu_write16(uint16_t addr, uint16_t data) {
  std::stringstream ss;
  ss << "Unmapped address for cpu_write16: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

void GB::clock() { cpu->clock(); }