#include "../include/GB.h"
#include <iostream>

int main() {
  GB gb("boot/dmg_boot.bin");
  while (true) {
    gb.clock();
  }
}