#include "core/cpu/cpu.h"
#include "cpu_test.h"

#ifdef IS_WINDOWS
    CPUTestAddressSpace* CPUTestAddressSpace::instance = nullptr;
#endif

uint8_t CPU::read8(uint16_t addr) { return CPUTestAddressSpace::get_instance()->get8(addr);}
uint16_t CPU::read16(uint16_t addr) { return CPUTestAddressSpace::get_instance()->get16(addr);}

void CPU::write8(uint16_t addr, uint8_t data) { CPUTestAddressSpace::get_instance()->set8(addr, data);}
void CPU::write16(uint16_t addr, uint16_t data) { CPUTestAddressSpace::get_instance()->set16(addr, data);}

void CPU::DI() {}
void CPU::EI() {}
