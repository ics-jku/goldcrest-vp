#include <iostream>
#include <iomanip>
#include <limits.h>

#include "core.hpp"
#include "riscv.hpp"
#include "microcode.hpp"

using namespace std;

GoldcrestCore::GoldcrestCore(Memory *memory): memory(memory) {

}


void GoldcrestCore::execute(uint32_t instruction) {
  auto opcode = prepare(instruction);
  pc = mapping[opcode];
  uint32_t instr;
  uint16_t src1, src2;
  int16_t jump = 0;

  while (jump != END) {
    instr = microcode[pc];
    src1 = instr_src1(instr);
    src2 = instr_src2(instr);
    jump = instr_target(instr);

    auto val_src2 = memory->load<int32_t>(src2);
    auto val_src1 = memory->load<int32_t>(src1);

    int64_t res64 = ((int64_t) val_src2) - ((int64_t) val_src1);
    int32_t res = (int32_t) res64;

    res = func(src2, res);
    memory->storew(src2, res);

    if (res64 <= 0) {
      pc += jump;
    } else {
      pc++;
    }

    cycles++;
  }
    
  finish(instruction);
  
  memory->storew(X0, 0);
}

opcode_t GoldcrestCore::prepare(uint32_t instruction) {
  auto opcode = riscv_decode(instruction);

  if (mapping.find(opcode) == mapping.end()) {
    stringstream stream;
    stream << hex << instruction;
    throw runtime_error("0x" + stream.str()  + " not implemented.");
  }
  
  switch (opcode) {
  case ADDI:
  case LB: case LBU:
  case LH: case LHU:
  case LW:
  case XORI:
  case ANDI:
  case ORI:
    memory->storew(IMM, instr_I_imm(instruction));
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    break;
  case SLTI:
  case SLTIU:
    memory->storew(SRC2, instr_I_imm(instruction));
    memory->storew(SRC1, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    break;    
  case ADD:
  case SUB:
  case XOR:
  case AND:
  case OR:
    memory->storew(IMM, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction))));
    break;
  case SLT:
  case SLTU:
    memory->storew(SRC1, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction))));
    break;
  case JAL:
    memory->storew(IMM, instr_J_imm(instruction));
    break;
  case JALR:
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(IMM, instr_I_imm(instruction));
    break;
  case SB:
  case SH:
  case SW:
    memory->storew(IMM, instr_S_imm(instruction));
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    break;
  case BLTU:
  case BLT:
  case BNE:
  case BEQ:
  case BGE:
    memory->storew(SRC1, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction))));
    memory->storew(IMM, instr_B_imm(instruction));
    break;
  case BGEU:
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(SRC1, memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction))));
    memory->storew(IMM, instr_B_imm(instruction));    
    break;
  case AUIPC:
  case LUI:
    memory->storew(IMM, instr_U_imm(instruction));
    break;
  case SLL:
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(SRC1, 0x1F & memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction)))); // better IMM
    break;
  case SRL: case SRA:
    memory->storew(SRC1, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(IMM, 0x1f & memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction))));
    break;
  case SRLI: case SRAI:
    memory->storew(SRC1, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(IMM, instr_I_imm(instruction) & 0x1F);
    break;
  case SLLI:
    //case SRLI:
    memory->storew(SRC2, memory->load<uint32_t>(reg_to_mem(instr_rs1(instruction))));
    memory->storew(SRC1, instr_I_imm(instruction) & 0x1F); // better IMM
    break;
  default:
    stringstream stream;
    stream << hex << instruction;
    throw runtime_error("0x" + stream.str()  + " not implemented.");
  }

  return opcode;
}

void GoldcrestCore::finish(uint32_t instruction) {
  auto opcode = riscv_decode(instruction);

  switch (opcode) {
  case ADDI: case ADD:
  case SUB:
  case AUIPC: case LUI:
  case JAL: case JALR:
  case SLL: case SLLI:
  case XOR: case XORI:
  case AND: case ANDI:
  case OR: case ORI:
  case SLT: case SLTI:
  case SLTU: case SLTIU:
  case SRL: case SRLI:
  case SRA: case SRAI:
    memory->storew(reg_to_mem(instr_rd(instruction)), memory->load<uint32_t>(SRC2));
    break;
  case LB:
    memory->storew(reg_to_mem(instr_rd(instruction)), (int32_t) memory->load<int8_t>(memory->load<uint32_t>(SRC2)));    
    break;
  case LBU:
    memory->storew(reg_to_mem(instr_rd(instruction)), (uint32_t) memory->load<uint8_t>(memory->load<uint32_t>(SRC2)));
    break;            
  case LH:
    memory->storew(reg_to_mem(instr_rd(instruction)), (int32_t) memory->load<int16_t>(memory->load<uint32_t>(SRC2)));
    break;
  case LHU:
    memory->storew(reg_to_mem(instr_rd(instruction)), (uint32_t) memory->load<uint16_t>(memory->load<uint32_t>(SRC2)));
    break;        
  case LW:
    memory->storew(reg_to_mem(instr_rd(instruction)), memory->load<int32_t>(memory->load<uint32_t>(SRC2)));
    break;
  case SB:
    memory->storeb(memory->load<uint32_t>(SRC2), memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction))));
    break;
  case SH:
    memory->storeh(memory->load<uint32_t>(SRC2), memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction))));
    break;
  case SW:
    memory->storew(memory->load<uint32_t>(SRC2), memory->load<uint32_t>(reg_to_mem(instr_rs2(instruction))));
    break;
  default:
    break;
  }

  memory->storew(SRC1, 0);
  memory->storew(SRC2, 0);
  memory->storew(IMM, 0);

  rv_instructions++;
}


void GoldcrestCore::dump_state() {
  cout << endl << "Goldcrest state" << endl;
  for (unsigned int reg = 0; reg < 32; ++reg) {
    printf("x%2d  = %8x\n", reg, memory->load<int32_t>(X0 + (reg * 4)));
  }

  printf(" pc  = %8x\n\n", memory->load<uint32_t>(PC));
  printf("src1 = %8x\n", memory->load<int32_t>(SRC1));
  printf("src2 = %8x\n", memory->load<int32_t>(SRC2));
  printf("imm  = %8x\n", memory->load<int32_t>(IMM));

  for (auto tmp = 0; tmp < 5; ++tmp) {
    printf("TMP%2d = %8d\n", tmp, memory->load<int32_t>(TMP0 + (tmp * 4)));
  }
  printf("Ran for  %lu cycles\n", cycles + memory->delay);
  printf("Executed %lu RISC-V instructions\n", rv_instructions);
  printf("%u cycles per RISC-V instruction\n", (unsigned int) (cycles/rv_instructions));  
}
