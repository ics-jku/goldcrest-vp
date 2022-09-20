#pragma once

#include "microcode.hpp"

#define BIT_RANGE(instr,upper,lower) (instr & (((1 << (upper-lower+1)) - 1) << lower))
#define instr_signature(instr) (((uint32_t) BIT_RANGE(instr, 31, 20)) >> 20)

#define instr_csr(instr) (BIT_RANGE((uint32_t)instr, 31, 20) >> 20)
#define instr_zimm(instr) (BIT_RANGE(instr, 19, 15) >> 15)
#define instr_shamt(instr) (BIT_RANGE(instr, 24, 20) >> 20)
#define instr_funct3(instr) (BIT_RANGE(instr, 14, 12) >> 12)
#define instr_funct12(instr) (BIT_RANGE((uint32_t)instr, 31, 20) >> 20)
#define instr_funct7(instr) (BIT_RANGE((uint32_t)instr, 31, 25) >> 25)
#define instr_funct5(instr) (BIT_RANGE((uint32_t)instr, 31, 27) >> 27)
#define instr_aq(instr) BIT_SINGLE(instr, 26)
#define instr_rl(instr) BIT_SINGLE(instr, 25)
#define instr_opcode(instr) BIT_RANGE(instr, 6, 0)
//#define instr_J_imm(instr) ((int32_t) ((BIT_SINGLE(instr,31) >> 11) | BIT_RANGE(instr,19,12) | (BIT_SINGLE(instr,20) >> 9) | (BIT_RANGE(instr,30,21) >> 20)))
int32_t instr_J_imm(int32_t instr);
int32_t instr_S_imm(int32_t instr);
int32_t instr_B_imm(int32_t instr);

//#define instr_B_imm(instr) (BIT_SINGLE(instr,31) >> 19) | (BIT_SINGLE(instr,7) << 4) | (BIT_RANGE(instr,30,25) >> 20) | (BIT_RANGE(instr,11,8) >> 7)
#define instr_I_imm(instr) (((int32_t) BIT_RANGE(instr,31,20)) >> 20)
//#define instr_S_imm(instr) (BIT_RANGE(instr,31,25) >> 20) | (BIT_RANGE(instr,11,7) >> 7)
#define instr_U_imm(instr) BIT_RANGE(instr,31,12)
#define instr_rs1(instr) (BIT_RANGE(instr,19,15) >> 15)
#define instr_rs2(instr) (BIT_RANGE(instr,24,20) >> 20)
#define instr_rd(instr) (BIT_RANGE(instr,11,7) >> 7)

enum Instruction {
  addi = 0b0000000'000'0010011,
  lw   = 0b0000000'010'0000011,
  sw   = 0b0000000'010'0100011
};


typedef enum {
    OP_LUI    = 0b0110111,
    OP_AUIPC  = 0b0010111,
    OP_JAL    = 0b1101111,
    OP_JALR   = 0b1100111,
    F3_JALR   = 0b000,

    OP_LB     = 0b0000011,
    F3_LB     = 0b000,
    F3_LH     = 0b001,
    F3_LW     = 0b010,
    F3_LBU    = 0b100,
    F3_LHU    = 0b101,

    OP_SB     = 0b0100011,
    F3_SB     = 0b000,
    F3_SH     = 0b001,
    F3_SW     = 0b010,

    OP_BEQ    = 0b1100011,
    F3_BEQ    = 0b000,
    F3_BNE    = 0b001,
    F3_BLT    = 0b100,
    F3_BGE    = 0b101,
    F3_BLTU   = 0b110,
    F3_BGEU   = 0b111,

    OP_ADDI   = 0b0010011,
    F3_ADDI   = 0b000,
    F3_SLTI   = 0b010,
    F3_SLTIU  = 0b011,
    F3_XORI   = 0b100,
    F3_ORI    = 0b110,
    F3_ANDI   = 0b111,
    F3_SLLI   = 0b001,
    F3_SRLI   = 0b101,
    F7_SRLI   = 0b0000000,
    F7_SRAI   = 0b0100000,

    OP_ADD    = 0b0110011,
    F7_ADD    = 0b0000000,
    F7_SUB    = 0b0100000,
    F3_ADD    = 0b000,
    F3_SUB    = 0b000,
    F3_SLL    = 0b001,
    F3_SLT    = 0b010,
    F3_SLTU   = 0b011,
    F3_XOR    = 0b100,
    F3_SRL    = 0b101,
    F3_SRA    = 0b101,
    F3_OR     = 0b110,
    F3_AND    = 0b111,
} opcode_part_t;

typedef enum {
    UNDEF = 0,

    // RV32I Base Instruction Set
    LUI = 1,
    AUIPC,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    SB,
    SH,
    SW,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI,
    SRLI,
    SRAI,
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    FENCE,
    ECALL,
    EBREAK,
    CSRRW,
    CSRRS,
    CSRRC,
    CSRRWI,
    CSRRSI,
    CSRRCI,

    // RV32M Standard Extension
    MUL,
    MULH,
    MULHSU,
    MULHU,
    DIV,
    DIVU,
    REM,
    REMU,

    // RV32A Standard Extension
    LR_W,
    SC_W,
    AMOSWAP_W,
    AMOADD_W,
    AMOXOR_W,
    AMOAND_W,
    AMOOR_W,
    AMOMIN_W,
    AMOMAX_W,
    AMOMINU_W,
    AMOMAXU_W,

    // privileged instructions
    URET,
    SRET,
    MRET,
    WFI,
    SFENCE_VMA,

    NUMBER_OF_INSTRUCTIONS
} opcode_t;

opcode_t riscv_decode(int32_t instr);
