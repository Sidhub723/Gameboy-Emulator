#include <iostream>
#include <string>
#include "core/interconnect/gb.h"

int main(int argc, char ** argv) {
  std::string boot_file_path = argv[1];
  std::string cartridge_file_path = argv[2];
  GB gb(boot_file_path, cartridge_file_path);
  while (true) {
    gb.clock();
  }
}