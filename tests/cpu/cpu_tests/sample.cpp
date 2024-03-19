#include <iostream>
#include "core/cpu/cpu.h"
#include "rw_log.h"
#include "cpu_test.h"

void cpu_sanity_check(CPU test_cpu)
{
    CPUTestAddressSpace::get_instance()->reset();
    CPUState state;
    test_cpu.set_state(state);
    test_cpu.read16(0x0000);
    state = test_cpu.get_state();
    std::cout << "Success" << std::endl;
}

int sample(int argc, char** argv)
{
    CPU test_cpu(nullptr);

    cpu_sanity_check(test_cpu);

    return 0;
}