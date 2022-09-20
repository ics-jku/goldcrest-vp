#include "microcode.hpp"
#include "core.hpp"

uint32_t reg_to_mem(char reg) {
  uint32_t res;
  switch (reg) {
  case 0: res = X0; break;
  case 1: res = X1; break;
  case 2: res = X2; break;
  case 3: res = X3; break;
  case 4: res = X4; break;
  case 5: res = X5; break;
  case 6: res = X6; break;
  case 7: res = X7; break;
  case 8: res = X8; break;
  case 9: res = X9; break;
  case 10: res = X10; break;
  case 11: res = X11; break;
  case 12: res = X12; break;
  case 13: res = X13; break;
  case 14: res = X14; break;
  case 15: res = X15; break;
  case 16: res = X16; break;
  case 17: res = X17; break;
  case 18: res = X18; break;
  case 19: res = X19; break;
  case 20: res = X20; break;
  case 21: res = X21; break;
  case 22: res = X22; break;
  case 23: res = X23; break;
  case 24: res = X24; break;
  case 25: res = X25; break;
  case 26: res = X26; break;
  case 27: res = X27; break;
  case 28: res = X28; break;
  case 29: res = X29; break;
  case 30: res = X30; break;
  case 31: res = X31; break;        
  }
  return res;
}
