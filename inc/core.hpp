#pragma once

#include <cstdint>
#include <array>
#include <unordered_map>

#include "riscv.hpp"
#include "memory.hpp"

class GoldcrestCore
{
  private:
  protected:
  uint16_t pc;
  Memory *memory;
  std::array<uint32_t, 1024> microcode;
  std::unordered_map<uint16_t, unsigned int> mapping;
  public:
  unsigned long int cycles = 0;
  unsigned long int rv_instructions = 0;
  GoldcrestCore(Memory *memory);
  void execute(uint32_t instruction);
  opcode_t prepare(uint32_t instruction);
  void finish(uint32_t instruction);  
  void dump_state();
  virtual void init() = 0;
  virtual int32_t func(uint32_t address, int32_t value) = 0;
};

