#ifndef CPU_TEST_H
#define CPU_TEST_H

#include <vector>
#include <cstdint>

class CPUTestAddressSpace
{
public:
    static CPUTestAddressSpace* get_instance()
    {
        if (instance == nullptr)
        {
            instance = new CPUTestAddressSpace();
        }
        return instance;
    }

    void reset()
    {
        memory.clear();
        memory.resize(0xFFFF);
    }

    uint8_t get8(uint16_t addr) { return memory[addr]; }
    void set8(uint16_t addr, uint8_t data) { memory[addr] = data; }

    uint16_t get16(uint16_t addr)
    {
        uint16_t data = memory[addr];
        data |= memory[addr + 1] << 8;
        return data;
    }

    void set16(uint16_t addr, uint16_t data)
    {
        memory[addr] = data & 0xFF;
        memory[addr + 1] = (data >> 8) & 0xFF;
    }

private:
    std::vector<uint8_t> memory;
    inline static CPUTestAddressSpace* instance;

private:
    CPUTestAddressSpace()
    {
        memory.resize(0xFFFF);
    }
};

class Bus
{

};

#endif