#ifndef GBA_H
#define GBA_H

#include<stdint.h>
#include<string>

class CPU;
class Boot;

class GBA {
    public :
        GBA(std::string boot_file_path);
        ~GBA();
        uint8_t cpu_read(uint16_t addr);
        void cpu_write(uint16_t addr, uint8_t data);
        void set_boot_mode(bool set_val){boot_mode_enabled = set_val;}
        // void set_boot_mode(bool set_val) : boot_mode_enabled(set_val){} doesnt work because its only
                                                                        // for constructors
        void clock();
    private :
        bool boot_mode_enabled;

        CPU *cpu;
        Boot *boot;

};




#endif