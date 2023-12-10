#include <iostream>
#include "../include/GBA.h"


int main()
{
    GBA gba("boot/dmg_boot.bin");
    while(true){
        gba.clock();
    }
}