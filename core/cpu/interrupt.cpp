#include <sstream>
#include <stdexcept>
#include "core/cpu/cpu.h"
#include "core/interconnect/gb.h"

void CPU::DI() {ime = false;}
void CPU::EI() {ime = true;}

void CPU::call_interrupt(uint16_t addr) {
  ime = false;
  write16(--SP, PC);
  PC = addr;
  cycles = 5;
}

void CPU::HALT() {
    Interrupt ie = gb->get_ie();
    Interrupt iflag = gb->get_iflag();

    if(ime)
    {
        if(ie.get_vblank() && iflag.get_vblank())
        {
            iflag.set_vblank(false);
            call_interrupt(0x40);
        }
        else if(ie.get_lcd_stat() && iflag.get_lcd_stat())
        {
            iflag.set_lcd_stat(false);
            call_interrupt(0x48);
        }
        else if(ie.get_timer() && iflag.get_timer())
        {
            iflag.set_timer(false);
            call_interrupt(0x50);
        }
        else if(ie.get_serial() && iflag.get_serial())
        {
            iflag.set_serial(false);
            call_interrupt(0x58);
        }
        else if(ie.get_joypad() && iflag.get_joypad())
        {
            iflag.set_joypad(false);
            call_interrupt(0x60);
        }

        gb->set_iflag(iflag);
    }
    else if((ie.reg & iflag.reg) != 0)
    {
        //halt bug
        halt_bug = true;
    }
}
