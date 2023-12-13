#include "../include/GBA.h"
#include "../include/Boot.h"
#include "../include/CPU.h"
#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

GBA::GBA(std::string boot_file_path) {
  boot_mode_enabled = true;
  boot = new Boot(boot_file_path);
  cpu = new CPU(this);
}
GBA::~GBA() {
  delete cpu;
  delete boot;
}
uint8_t GBA::cpu_read(uint16_t addr) {

  if (boot_mode_enabled && boot_range.in_range(addr)) {
    return boot->read8(boot_range.offset_of(addr));
  }

  std::stringstream ss;
  ss << "Unmapped address for cpu_read: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

void GBA::cpu_write(uint16_t addr, uint8_t data) {}

void GBA::clock() { cpu->clock(); }
