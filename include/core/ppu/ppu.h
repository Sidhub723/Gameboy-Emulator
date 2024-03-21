#ifndef PPU_H
#define PPU_H

#include <cstdint>

class GB;

struct LCDC
{
    LCDC() {}
    LCDC(uint8_t reg_val) {reg = reg_val;}
    uint8_t reg;

    bool get_lcd_enable() {return reg & 0x80;}
    void set_lcd_enable(bool val) {(val ? (reg = reg | 0x80) : (reg = reg & (~0x80)));}

    bool get_win_tm_area() {return reg & 0x40;}
    void set_win_tm_area(bool val) {(val ? (reg = reg | 0x40) : (reg = reg & (~0x40)));}

    bool get_win_enable() {return reg & 0x20;}
    void set_win_enable(bool val) {(val ? (reg = reg | 0x20) : (reg = reg & (~0x20)));}

    bool get_bg_win_td_area() {return reg & 0x10;}
    void set_bg_win_td_area(bool val) {(val ? (reg = reg | 0x10) : (reg = reg & (~0x10)));}

    bool get_bg_tm_area() {return reg & 0x08;}
    void set_bg_tm_area(bool val) {(val ? (reg = reg | 0x08) : (reg = reg & (~0x08)));}

    bool get_obj_size() {return reg & 0x04;}
    void set_obj_size(bool val) {(val ? (reg = reg | 0x04) : (reg = reg & (~0x04)));}

    bool get_obj_enable() {return reg & 0x02;}
    void set_obj_enable(bool val) {(val ? (reg = reg | 0x02) : (reg = reg & (~0x02)));}

    bool get_bg_win_priority() {return reg & 0x01;}
    void set_bg_win_priority(bool val) {(val ? (reg = reg | 0x01) : (reg = reg & (~0x01)));}
};

struct STAT
{
    STAT() {}
    STAT(uint8_t reg_val) {reg = reg_val;}
    uint8_t reg;

    bool get_lyc_int_select() {return reg & 0x40;}
    void set_lyc_int_select(bool val) {(val ? (reg = reg | 0x40) : (reg = reg & (~0x40)));}

    bool get_mode2_int_select() {return reg & 0x20;}
    void set_mode2_int_select(bool val) {(val ? (reg = reg | 0x20) : (reg = reg & (~0x20)));}

    bool get_mode1_int_select() {return reg & 0x10;}
    void set_mode1_int_select(bool val) {(val ? (reg = reg | 0x10) : (reg = reg & (~0x10)));}

    bool get_mode0_int_select() {return reg & 0x08;}
    void set_mode0_int_select(bool val) {(val ? (reg = reg | 0x08) : (reg = reg & (~0x08)));}

    bool get_lyc_equal_lc() {return reg & 0x04;}

    uint8_t get_ppu_mode() {return reg & 0x03;}
};

struct Palette
{
    Palette() {}
    Palette(uint8_t reg_val) {reg = reg_val;}
    uint8_t reg;

    uint8_t get_color(uint8_t id)
    {
        switch(id)
        {
            case 0:
                return reg & 0x03; break;
            case 1:
                return reg & 0x0c; break;
            case 2:
                return reg & 0x30; break;
            case 3:
                return reg & 0xc0; break;
            default:
                break;
        }
    }
};

class PPU
{
public:
    PPU (GB* gb);
    ~PPU() {};

    void write8(uint16_t addr, uint8_t data);
    uint8_t read8(uint16_t addr);

private:
    GB* gb;

public:
    LCDC lcdc;
    uint8_t LY;
    uint8_t LYC;
    STAT stat;
    uint8_t SCY;
    uint8_t SCX;
    uint8_t WY;
    uint8_t WX;
    Palette bgp;
    Palette objp0;
    Palette objp1;
};

#endif