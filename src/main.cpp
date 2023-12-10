#include "../include/GBA.h"
#include <iostream>

int main() {
  GBA gba("boot/dmg_boot.bin");
  while (true) {
    gba.clock();
  }
}