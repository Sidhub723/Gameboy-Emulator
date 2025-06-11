#ifndef GB_H
#define GB_H

#include <stdint.h>
#include <cassert>
#include <string>

#define BOOT_RANGE 0x0000, 0x00FF
#define ROM_RANGE 0x0000, 0x7FFF
#define VRAM_RANGE 0x8000, 0x9FFF
#define ERAM_RANGE 0xA000, 0xBFFF
#define WRAM_RANGE 0xC000, 0xDFFF
#define OAM_RANGE 0xFE00, 0xFE9F
#define IO_RANGE 0xFF00, 0xFF7F
#define HRAM_RANGE 0xFF80, 0xFFFE
#define IE_RANGE 0xFFFF, 0xFFFF


class CPU;
class Boot;
class ROM;
class VRAM;
class IO;
class HRAM;

struct Range
{
  uint16_t start;
  uint16_t end;
  Range(uint16_t start, uint16_t end) : start(start), end(end) {}

  bool in_range(uint16_t addr) { return addr >= start && addr <= end; }
  uint16_t offset_of(uint16_t addr) { assert(in_range(addr)); return addr - start; }
};

struct Interrupt
{
  Interrupt() {}
  Interrupt(uint8_t reg_val): reg(reg_val) {}
  uint8_t reg;

  bool get_vblank() { return reg & 0x01; }
  void set_vblank(bool val) { (val ? (reg = reg | 0x01) : (reg = reg & (~0x01))); }

  bool get_lcd_stat() { return reg & 0x02; }
  void set_lcd_stat(bool val) { (val ? (reg = reg | 0x02) : (reg = reg & (~0x02))); }

  bool get_timer() { return reg & 0x04; }
  void set_timer(bool val) { (val ? (reg = reg | 0x04) : (reg = reg & (~0x04))); }

  bool get_serial() { return reg & 0x08; }
  void set_serial(bool val) { (val ? (reg = reg | 0x08) : (reg = reg & (~0x08))); }

  bool get_joypad() { return reg & 0x10; }
  void set_joypad(bool val) { (val ? (reg = reg | 0x10) : (reg = reg & (~0x10))); }
};

class GB {
public:
  GB(std::string boot_file_path, std::string cartridge_file_path);
  ~GB();
  
  uint8_t read8(uint16_t addr);
  void write8(uint16_t addr, uint8_t data);
  uint16_t read16(uint16_t addr);
  void write16(uint16_t addr, uint16_t data);
  
  void clock();
  
  Interrupt get_ie() { return ie; }
  void set_ie(Interrupt val) { ie = val; }
  Interrupt get_iflag() { return iflag; }
  void set_iflag(Interrupt val) { iflag = val; }

private:
  Interrupt ie;
  Interrupt iflag;

private:
  bool boot_mode_enabled;

  CPU *cpu;
  Boot *boot;
  ROM *rom;
  VRAM *vram;
  IO *io;
  HRAM *hram;

private:
  Range boot_range = Range(BOOT_RANGE);
  Range rom_range = Range(ROM_RANGE);
  Range vram_range = Range(VRAM_RANGE);
  Range eram_range = Range(ERAM_RANGE);
  Range wram_range = Range(WRAM_RANGE);
  Range oam_range = Range(OAM_RANGE);
  Range io_range = Range(IO_RANGE);
  Range hram_range = Range(HRAM_RANGE);
  Range ie_range = Range(IE_RANGE);
};

#endif
