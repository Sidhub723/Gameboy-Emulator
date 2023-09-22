#include <fstream>

#include "../include/Boot.h"

Boot::Boot(std::string file_path)
{
    std::ifstream file(file_path, std::ios::binary);

    //check file size and throw error if not 256 bytes
    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    if(file_size != 256)
    {
        throw std::runtime_error("Boot ROM file is not 256 bytes");
    }

    file.seekg(0, std::ios::beg);
    //Read 256 bytes of the file into boot_rom
    for (int i = 0; i < 256; i++)
    {
        boot_rom[i] = file.get();
    }
}

uint8_t Boot::read8(uint16_t offset)
{
    return boot_rom[offset];
}