#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <core/memory/rom.h>


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

uint8_t ROM::read8(uint16_t offset) {
    return rom[offset];
}

uint16_t ROM::read16(uint16_t offset) {
    return *((uint16_t*)(rom + offset));
}
