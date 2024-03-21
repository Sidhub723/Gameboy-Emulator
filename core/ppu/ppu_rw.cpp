#include <cstdint>
#include <sstream>
#include <iostream>

#include "core/ppu/ppu.h"

uint8_t PPU::read8(uint16_t addr)
{
    switch(addr)
    {
        case 0xff40:
            return lcdc.reg; break;
        case 0xff41:
            return stat.reg; break;
        case 0xff42:
            return SCY; break;
        case 0xff43:
            return SCX; break;
        case 0xff44:
            return LY; break;
        case 0xff45:
            return LYC; break;
        case 0xff47:
            return bgp.reg; break;
        case 0xff48: 
            return objp0.reg; break;
        case 0xff49:
            return objp1.reg; break;
        case 0xff4a:
            return WY; break;
        case 0xff4b:
            return WX; break;
        default:
            std::stringstream ss;
            ss << "Unmapped address for read8 (ppu): 0x" << std::hex << addr;
            throw std::runtime_error(ss.str());
    }
}