#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>
#include <string>

class Boot
{
public:
    Boot(std::string file_path);
    uint8_t read8(uint16_t offset);

private:
    uint8_t boot_rom[0x100];
};

#endif