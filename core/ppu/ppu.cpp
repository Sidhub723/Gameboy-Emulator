#include <cstdint>

#include "core/ppu/ppu.h"

PPU::PPU(GB* gb_p)
{
    this->gb = gb_p;
}