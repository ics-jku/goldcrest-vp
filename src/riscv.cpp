#include "riscv.hpp"

opcode_t riscv_decode(int32_t instr) {
  switch (instr_opcode(instr)) {
  case OP_LUI:
    return LUI;
    
  case OP_AUIPC:
    return AUIPC;
    
  case OP_JAL:
    return JAL;

  case OP_JALR: {
    return JALR;
  }

  case OP_LB: {
    switch (instr_funct3(instr)) {
    case F3_LB:
      return LB;
    case F3_LH:
      return LH;
    case F3_LW:
      return LW;
    case F3_LBU:
      return LBU;
    case F3_LHU:
      return LHU;
    }
    break;
  }

  case OP_SB: {
    switch (instr_funct3(instr)) {
    case F3_SB:
      return SB;
    case F3_SH:
      return SH;
    case F3_SW:
      return SW;
    }
    break;
  }

    
  case OP_BEQ: {
    switch (instr_funct3(instr)) {
    case F3_BEQ:
      return BEQ;
    case F3_BNE:
      return BNE;
    case F3_BLT:
      return BLT;
    case F3_BGE:
      return BGE;
    case F3_BLTU:
      return BLTU;
    case F3_BGEU:
      return BGEU;
    }
    break;
  }

  case OP_ADDI: {
    switch (instr_funct3(instr)) {
    case F3_ADDI:
      return ADDI;
    case F3_SLTI:
      return SLTI;
    case F3_SLTIU:
      return SLTIU;
    case F3_XORI:
      return XORI;
    case F3_ORI:
      return ORI;
    case F3_ANDI:
      return ANDI;
    case F3_SLLI:
      return SLLI;
    case F3_SRLI: {
      switch (instr_funct7(instr)) {
      case F7_SRLI:
	return SRLI;
      case F7_SRAI:
	return SRAI;
      }
    }
    }
    break;
  }

  case OP_ADD: {
    switch (instr_funct7(instr)) {
    case F7_ADD:
      switch (instr_funct3(instr)) {
      case F3_ADD:
	return ADD;
      case F3_SLL:
	return SLL;
      case F3_SLT:
	return SLT;
      case F3_SLTU:
	return SLTU;
      case F3_XOR:
	return XOR;
      case F3_SRL:
	return SRL;
      case F3_OR:
	return OR;
      case F3_AND:
	return AND;
      }
      break;

    case F7_SUB:
      switch (instr_funct3(instr)) {
      case F3_SUB:
	return SUB;
      case F3_SRA:
	return SRA;
      }
      break;
    }
    break;
  }
  }
  return UNDEF;
}

int32_t instr_J_imm(int32_t instr) {
  return (BIT_SINGLE(instr,31) >> 11) | BIT_RANGE(instr,19,12) | (BIT_SINGLE(instr,20) >> 9) | (BIT_RANGE(instr,30,21) >> 20);
}

int32_t instr_S_imm(int32_t instr) {
  return (BIT_RANGE(instr,31,25) >> 20) | (BIT_RANGE(instr,11,7) >> 7);
}

int32_t instr_B_imm(int32_t instr) {
  return (BIT_SINGLE(instr,31) >> 19) | (BIT_SINGLE(instr,7) << 4) | (BIT_RANGE(instr,30,25) >> 20) | (BIT_RANGE(instr,11,8) >> 7);
}
