#include <iostream>

#include "memory.hpp"
#include "microcode.hpp"
#include "elf_loader.hpp"

Memory::Memory(size_t size): size(size), data(new uint8_t[size]()) {}

uint32_t Memory::load_elf(std::string filename) {
  ELFLoader loader(filename.c_str());
  loader.load_executable_image(data, size, 0x0);

  storew(PC, loader.get_entrypoint());
  storew(NEXT, -4);
  storew(INC, -1);
  storew(ONE, 1);
  storew(TWO, 2);
  storew(WORD, 31);

  return loader.get_entrypoint();
}

uint32_t Memory::fetch() {
  return load<uint32_t>(load<uint32_t>(PC));
}

void Memory::storeb(uint32_t address, int8_t word) {
  store(address, word, 1);
}

void Memory::storeh(uint32_t address, int16_t word) {
  store(address, word, 2);
}

void Memory::storew(uint32_t address, int32_t word) {
  store(address, word, 4);
}
  
void Memory::store(uint32_t address, int32_t word, size_t size) {
  if (address == 1024) {
    // MMIO style print out character when written to this address
    printf("%c", (char) word);
    fflush(stdout);
  } else if (address == 1016) {
    // MMIO style print out integer when written to this address
    printf("%d", word);
    fflush(stdout);
  } else if (address == 1020) {
    // MMIO style stop the core when written to this address
    stopped = true;
  } else if (address == MSB) {
    // store only MSB
    word >>= 31;
    //word = 0xFFFFFFFE | (word >> 31);
    std::memcpy(data + address, &word, size);
  } else {
    std::memcpy(data + address, &word, size);
  }

  delay += calc_delay(address);
}

