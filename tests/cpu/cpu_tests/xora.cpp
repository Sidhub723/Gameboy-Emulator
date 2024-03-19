#include <iostream>
#include "core/cpu/cpu.h"
#include "rw_log.h"
#include "cpu_test.h"
#include <random>

void operation_check(CPU* test_cpu)
{
    uint8_t opcode = 0xa8;
    uint8_t num_tests = 20;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dis(0x00, 0xff);

    //testing XOR for 20 random values
    for(uint8_t i = 0; i < num_tests; i++)
    {
        CPUTestAddressSpace::get_instance()->reset();
        CPUTestAddressSpace::get_instance()->set8(0x0000, opcode);

        CPUState state;
        state.PC = 0x0000;
        state.AF.hi = dis(gen);
        state.BC.hi = dis(gen);
        uint8_t result = state.AF.hi ^ state.BC.hi;

        test_cpu->set_state(state);
        test_cpu->clock();
        state = test_cpu->get_state();

        if(result != state.AF.hi)
        {
            std::cout << "XORA Operation (Register): Failure" << std::endl;
            return;
        }
    }

    opcode = 0xee;
    
    //testing XOR for 20 random values
    for(uint8_t i = 0; i < num_tests; i++)
    {
        uint8_t randimm = dis(gen);
        CPUTestAddressSpace::get_instance()->reset();
        CPUTestAddressSpace::get_instance()->set8(0x0000, opcode);
        CPUTestAddressSpace::get_instance()->set8(0x0001, randimm);

        CPUState state;
        state.PC = 0x0000;
        state.AF.hi = dis(gen);
        uint8_t result = state.AF.hi ^ randimm;

        test_cpu->set_state(state);
        test_cpu->clock();
        test_cpu->clock();
        state = test_cpu->get_state();

        if(result != state.AF.hi)
        {
            std::cout << "XORA Operation (Immediate): Failure" << std::endl;
            return;
        }
    }

    std::cout << "XORA Operation: Success" << std::endl;
}

void timing_register_check(CPU* test_cpu)
{
    uint8_t opcode = 0xa8;

    CPUTestAddressSpace::get_instance()->reset();
    CPUTestAddressSpace::get_instance()->set8(0x0000, opcode);

    CPUState state;
    state.PC = 0x0000;
    state.AF.full = 0x0000;
    state.BC.full = 0xab00;

    test_cpu->set_state(state);
    test_cpu->clock();
    test_cpu->clock();
    state = test_cpu->get_state();


    if(state.PC != 0x0002)
    {
        std::cout << "XORA Timing (Register): Failure" << std::endl;
        return;
    }

    std::cout << "XORA Timing (Register): Success" << std::endl;
}

void timing_memory_check(CPU* test_cpu)
{
    uint8_t opcode = 0xae;

    CPUTestAddressSpace::get_instance()->reset();
    CPUTestAddressSpace::get_instance()->set8(0x0000, opcode);

    CPUState state;
    state.PC = 0x0000;
    state.AF.full = 0x0000;

    test_cpu->set_state(state);
    test_cpu->clock();
    test_cpu->clock();
    state = test_cpu->get_state();


    if(state.PC != 0x0001)
    {
        std::cout << "XORA Timing (Memory): Failure" << std::endl;
        return;
    }

    std::cout << "XORA Timing (Memory): Success" << std::endl;
}

void operand_register_check(CPU* test_cpu)
{
    uint8_t opcode = 0xa8;
    CPUState* state = new CPUState();

    uint8_t* operand_registers[] = {&state->BC.hi, &state->BC.lo , &state->DE.hi, &state->DE.lo, &state->HL.hi, &state->HL.lo, nullptr, &state->AF.hi};
    for(uint8_t i = 0; i < 8; i++)
    {
        if(i == 6) continue;

        CPUTestAddressSpace::get_instance()->reset();
        CPUTestAddressSpace::get_instance()->set8(0x0000, opcode + i);
        
        state->PC = 0x0000;
        state->AF.full = 0x0000;
        *operand_registers[i] = 0xcf;

        test_cpu->set_state(*state);
        test_cpu->clock();
        *state = test_cpu->get_state();

        if((state->AF.hi != 0xcf && i < 7) || (state->AF.hi != 0x00 && i == 7))
        {
            std::cout << "XORA Operand (Register): Failure" << std::endl;
            return;
        }
    }

    std::cout << "XORA Operand (Memory): Success" << std::endl;
}

void operand_memory_check(CPU* test_cpu)
{
    uint8_t opcode = 0xae;

    CPUTestAddressSpace::get_instance()->reset();
    CPUTestAddressSpace::get_instance()->set8(0x0000, opcode);
    CPUTestAddressSpace::get_instance()->set8(0xbeef, 0x0f);

    CPUState state;
    state.PC = 0x0000;
    state.AF.full = 0x0000;
    state.HL.full = 0xbeef;

    test_cpu->set_state(state);
    test_cpu->clock();
    state = test_cpu->get_state();

    if(state.AF.full != 0x0f00)
    {
        std::cout << "XORA Memory Operand: Failure" << std::endl;
        return;
    }

    std::cout << "XORA Memory Operand: Success" << std::endl;
}

void operand_immediate_check(CPU* test_cpu)
{
    uint8_t opcode = 0xee;

    CPUTestAddressSpace::get_instance()->reset();
    CPUTestAddressSpace::get_instance()->set8(0x0000, opcode);
    CPUTestAddressSpace::get_instance()->set8(0x0001, 0x0f);

    CPUState state;
    state.PC = 0x0000;
    state.AF.full = 0x0000;

    test_cpu->set_state(state);
    test_cpu->clock();
    test_cpu->clock();
    state = test_cpu->get_state();

    if(state.AF.full != 0x0f00)
    {
        std::cout << "XORA Operand (Immediate): Failure" << std::endl;
        return;
    }

    std::cout << "XORA Operand (Immediate): Success" << std::endl;
}

void flag_check(CPU* test_cpu)
{
    uint8_t opcode = 0xa8;

    CPUTestAddressSpace::get_instance()->reset();
    CPUTestAddressSpace::get_instance()->set8(0x0000, opcode);

    //set zero flag
    CPUState state;
    state.PC = 0x0000;
    state.AF.full = 0x0000;
    state.BC.full = 0xbe00;

    test_cpu->set_state(state);
    test_cpu->clock();
    state = test_cpu->get_state();

    if(state.AF.lo != 0x00)
    {
        std::cout << "XORA Flag: Failure" << std::endl;
        return;
    }

    state.PC = 0x0000;
    state.AF.full = 0xbe00;
    state.BC.full = 0xbe00;

    test_cpu->set_state(state);
    test_cpu->clock();
    state = test_cpu->get_state();

    if(state.AF.lo != 0x80)
    {
        std::cout << "XORA Flag: Failure" << std::endl;
        return;
    }

    std::cout << "XORA Flag: Success" << std::endl;
}

int xora(int argc, char** const argv)
{
    CPU* test_cpu = new CPU(nullptr);
    std::cout << "Testing XORA..." << std::endl;
    test_cpu->read16(0x0000); //fixes a reference bug

    operation_check(test_cpu);
    timing_register_check(test_cpu);
    timing_memory_check(test_cpu);
    operand_register_check(test_cpu);
    operand_memory_check(test_cpu);
    operand_immediate_check(test_cpu);
    flag_check(test_cpu);

    return 0;
}