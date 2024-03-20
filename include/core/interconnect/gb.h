#ifndef GB_H
#define GB_H

#include <stdint.h>
#include <string>

#define BOOT_RANGE 0x0000, 0x0100
#define VRAM_RANGE 0x8000, 0xA000
#define IO_RANGE 0xFF00, 0xFF7F
#define ROM_RANGE 0x0000, 0x8000

class CPU;
class Boot;
class VRAM;
class IO;
class ROM;

struct Range
{
  uint16_t start;
  uint16_t end;
  Range(uint16_t start, uint16_t end) : start(start), end(end) {}

  bool in_range(uint16_t addr) { return addr >= start && addr < end; }
  uint16_t offset_of(uint16_t addr) { return addr - start; }
};

class GB {
public:
  GB(std::string boot_file_path, std::string cartridge_file_path);
  ~GB();
  uint8_t cpu_read8(uint16_t addr);
  void cpu_write8(uint16_t addr, uint8_t data);
  uint16_t cpu_read16(uint16_t addr);
  void cpu_write16(uint16_t addr, uint16_t data);
  void set_boot_mode(bool set_val) { boot_mode_enabled = set_val; }
  // void set_boot_mode(bool set_val) : boot_mode_enabled(set_val){} doesnt work
  // because its only for constructors
  void clock();

private:
  bool boot_mode_enabled;

  CPU *cpu;
  Boot *boot;
  VRAM *vram;
  IO *io;
  ROM *rom;

private:
  Range boot_range = Range(BOOT_RANGE);
  Range vram_range = Range(VRAM_RANGE);
  Range io_range = Range(IO_RANGE);
  Range rom_range = Range(ROM_RANGE);
};

#endif