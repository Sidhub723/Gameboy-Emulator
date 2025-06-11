#include <sstream>
#include <stdexcept>
#include "core/interconnect/gb.h"
#include "core/cpu/cpu.h"
#include "core/boot/boot.h"
#include "core/memory/rom.h"
#include "core/memory/vram.h"
#include "core/memory/io.h"
#include "core/memory/hram.h"

GB::GB(std::string boot_file_path, std::string cartridge_file_path) {
  cpu = new CPU(this);
  
  boot_mode_enabled = true;
  boot = new Boot(boot_file_path);

  rom = new ROM(cartridge_file_path);
  vram = new VRAM();
  io = new IO();
  hram = new HRAM();
}

GB::~GB() {
  delete boot;
  delete cpu;
  delete rom;
  delete vram;
  delete io;
  delete hram;
}

uint8_t GB::read8(uint16_t addr) {
  if (boot_mode_enabled && boot_range.in_range(addr)) {
    return boot->read8(boot_range.offset_of(addr));
  }
  else if (rom_range.in_range(addr)) {
    return rom->read8(rom_range.offset_of(addr));
  }
  else if (vram_range.in_range(addr)) {
    return vram->read8(vram_range.offset_of(addr));
  }
  else if (io_range.in_range(addr)) {
    return io->read8(io_range.offset_of(addr));
  }
  else if (hram_range.in_range(addr)) {
    return hram->read8(hram_range.offset_of(addr));
  }
  else if (ie_range.in_range(addr)) {
    return ie.reg;
  }

  std::stringstream ss;
  ss << "Unmapped address for read8: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

uint16_t GB::read16(uint16_t addr) {
  if (boot_mode_enabled && boot_range.in_range(addr)) {
    return boot->read16(boot_range.offset_of(addr));
  }
  else if (rom_range.in_range(addr)) {
    return rom->read16(rom_range.offset_of(addr));
  }
  else if (vram_range.in_range(addr)) {
    return vram->read16(vram_range.offset_of(addr));
  }
  else if (io_range.in_range(addr)) {
    return io->read16(io_range.offset_of(addr));
  }
  else if (hram_range.in_range(addr)) {
    return hram->read16(hram_range.offset_of(addr));
  }

  std::stringstream ss;
  ss << "Unmapped address for read16: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

void GB::write8(uint16_t addr, uint8_t data) {
  if (vram_range.in_range(addr)) {
    vram->write8(vram_range.offset_of(addr), data);
    return;
  }
  else if (io_range.in_range(addr)) {
    io->write8(io_range.offset_of(addr), data);
    return;
  }
  else if (hram_range.in_range(addr)) {
    hram->write8(hram_range.offset_of(addr), data);
    return;
  }
  else if (ie_range.in_range(addr)) {
    ie.reg = data;
    return;
  }
  
  std::stringstream ss;
  ss << "Unmapped address for write8: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

void GB::write16(uint16_t addr, uint16_t data) {
  if (vram_range.in_range(addr)) {
    vram->write16(vram_range.offset_of(addr), data);
    return;
  }
  else if (io_range.in_range(addr)) {
    io->write16(io_range.offset_of(addr), data);
    return;
  }
  else if (hram_range.in_range(addr)) {
    hram->write16(hram_range.offset_of(addr), data);
    return;
  }

  std::stringstream ss;
  ss << "Unmapped address for write16: 0x" << std::hex << addr;
  throw std::runtime_error(ss.str());
}

void GB::clock() { cpu->clock(); }
