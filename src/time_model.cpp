#include "memory.hpp"

#if REG_IMPL == REG
unsigned int Memory::calc_delay(uint32_t address) {
  if ((address >= SRC1 && address <= WORD) || (address >= PC && address <= X31)) {
    return 0;
  } else {
    return 4;
  }
}
#elif REG_IMPL == MEM
unsigned int Memory::calc_delay(uint32_t address) {
  return 4;
}
#elif REG_IMPL == BRAM
unsigned int Memory::calc_delay(uint32_t address) {
  if ((address >= SRC1 && address <= WORD) || (address >= PC && address <= X31)) {    
    return 1;
  } else {
    return 4;
  }
}
#elif REG_IMPL == TOP
unsigned int Memory::calc_delay(uint32_t address) {
  switch (address) {
  case SRC1: case SRC2:
  case TMP0: case TMP1:
    return 0;
  default:
    return 4;
  }
}
#elif REG_IMPL == TOPPC
unsigned int Memory::calc_delay(uint32_t address) {
  switch (address) {
  case PC:
  case SRC1: case SRC2:
  case TMP0: case TMP1:
    return 0;
  default:
    return 4;
  }
}
#elif REG_IMPL == RISCV
unsigned int Memory::calc_delay(uint32_t address) {
  if (address >= PC && address <= X31) {    
    return 0;
  } else {
    return 4;
  }
}
#else
unsigned int Memory::calc_delay(uint32_t address) {
  return 4;
}
#endif
