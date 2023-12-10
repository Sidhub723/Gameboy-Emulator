#include "../include/CPU.h"
#include "../include/GBA.h"
#include <stdexcept>         //for throwing runtime errors
#include <sstream>           //to format error output nicely
#include <iostream>
CPU::CPU(GBA *gba): gba(gba){

    PC = 0x0000;
    AF = BC = DE = HL = SP = 0x0000;
    cycles = 0;
    op = 0;

    //setting up the instruction map

}

void CPU::clock(){
    if(!cycles){
        //fetch
        read_ins();

        //decode

        if(instruction_map.find(op) == instruction_map.end()){
            std::stringstream ss;
            ss << "Instruction not implemented: 0x" << std::hex << (int)op;
            throw std::runtime_error(ss.str());
        }
        //execute
        (this->*(instruction_map[op].first))();
        cycles = instruction_map[op].second;
    }
    cycles--;
}

uint8_t CPU::read8(uint16_t addr){
    return gba->cpu_read(addr);
}

void CPU::read_ins(){
    op = read8(PC++);
}

