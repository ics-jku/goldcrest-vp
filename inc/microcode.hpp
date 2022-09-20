#pragma once

#include <cstdint>

#define PC 0
#define RVPC 0
#define X0 4
#define X1 8
#define X2 12
#define X3 16
#define X4 20
#define X5 24
#define X6 28
#define X7 32
#define X8 36
#define X9 40
#define X10 44
#define X11 48
#define X12 52
#define X13 56
#define X14 60
#define X15 64
#define X16 68
#define X17 72
#define X18 76
#define X19 80
#define X20 84
#define X21 88
#define X22 92
#define X23 96
#define X24 100
#define X25 104
#define X26 108
#define X27 112
#define X28 116
#define X29 120
#define X30 124
#define X31 128

#define SRC1 132
#define SRC2 136
#define RSLT SRC2
#define TARGET 140
#define IMM 144

#define TMP0 148
#define TMP1 152
#define TMP2 156
#define TMP3 160
#define TMP4 164
#define TMP5 168

#define NEXT 172
#define INC 176
#define MSB 180
#define LSB 184

#define ZERO X0
#define ONE 188
#define TWO 192
#define WORD 196

#define FUNC1 200
#define FUNC2 204
#define FUNC3 208
#define FUNC4 212

#define END -1

#define ZERO X0
#define RA X1
#define SP X2
#define GP X3
#define TP X4

#define build(src1, src2, target) ((src1 & 0b111111111111) << 20) | ((src2 & 0b111111111111) << 8) | (target &0b11111111)
#define instr(src1, src2) ((src1 & 0b111111111111) << 20) | ((src2 & 0b111111111111) << 8) | (1 &0b11111111)
#define clean(src) build(src, src, 1)

#define BIT_RANGE(instr,upper,lower) (instr & (((1 << (upper-lower+1)) - 1) << lower))
#define BIT_SINGLE(instr,pos) (instr & (1 << pos))
#define instr_src1(instr) (((uint32_t) BIT_RANGE(instr, 31, 20)) >> 20)
#define instr_src2(instr) (((uint32_t) BIT_RANGE(instr, 19, 8)) >> 8)
#define instr_target(instr) ((int8_t) BIT_RANGE(instr, 7, 0))

uint32_t reg_to_mem(char reg);
