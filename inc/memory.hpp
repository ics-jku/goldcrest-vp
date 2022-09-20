#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>

#include "microcode.hpp"

class Memory {
private:
  size_t size;
  uint8_t *data;
protected:
public:
  unsigned int delay = 0;
  bool stopped = false;
  Memory(size_t size);
  uint32_t load_elf(std::string filename);
  uint32_t fetch();
  void storeb(uint32_t address, int8_t word);
  void storeh(uint32_t address, int16_t word);
  void storew(uint32_t address, int32_t word);  
  void store(uint32_t address, int32_t word, size_t size);
  unsigned int calc_delay(uint32_t address);
  
  template <class T>
  T load(uint32_t address) {
    delay += calc_delay(address);
    return *(T*) (data + address); 
  }
};
