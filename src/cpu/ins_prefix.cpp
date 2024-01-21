#include "../../include/CPU.h"


#include <sstream>   //to format error output nicely
#include <stdexcept> //for throwing runtime errors

//SECTION - PREFIX INSTRUCTIONS

void CPU::PFX()
{
  read_ins();
}