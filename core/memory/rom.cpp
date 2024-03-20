#include <fstream>
#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors
#include <core/memory/rom.h>

#include <iostream>

ROM::ROM(std::string file_path) {
    std::ifstream file(file_path, std::ios::binary);

    file.seekg(0, std::ios::end);
    int file_size = file.tellg();

    rom = new uint8_t[file_size];

    file.seekg(0, std::ios::beg);
    for (int i = 0; i < file_size; i++) {
        rom[i] = file.get();
    }

    std::cout << "ROM loaded: " << file_size << " bytes" << std::endl;
}

uint8_t ROM::read8(uint16_t addr) {
    if (addr >= 0x0000 && addr < 0x8000) {
        return rom[addr];
    }
    else {
        std::stringstream ss;
        ss << "Unmapped address for rom_read8: 0x" << std::hex << addr;
        throw std::runtime_error(ss.str());
    }
}

uint16_t ROM::read16(uint16_t addr) {
    if (addr < 0x8000) {
        return *((uint16_t*)(rom + addr));
    }
    else {
        std::stringstream ss;
        ss << "Unmapped address for rom_read16: 0x" << std::hex << addr;
        throw std::runtime_error(ss.str());
    }
}