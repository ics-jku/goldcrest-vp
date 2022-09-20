#pragma once

#include "../inc/core.hpp"

class CompliantCore : GoldcrestCore {
public:
  CompliantCore(Memory *memory): GoldcrestCore(memory) {
    init();
  }

  void init() {
    // # ============================================================
    // #                         ARITHMETIC
    mapping[  LB] = 0;
    // Instruction LB
    // LB:
    mapping[  LH] = 0;
    // Instruction LH
    // LH:
    mapping[  LW] = 0;
    // Instruction LW
    // LW:
    mapping[ LBU] = 0;
    // Instruction LBU
    // LBU:
    mapping[ LHU] = 0;
    // Instruction LHU
    // LHU:
    mapping[  SB] = 0;
    // Instruction SB
    // SB:
    mapping[  SH] = 0;
    // Instruction SH
    // SH:
    mapping[  SW] = 0;
    // Instruction SW
    // SW:
    mapping[ADDI] = 0;
    // Instruction ADDI
    // ADDI:
    mapping[ ADD] = 0;
    // Instruction ADD
    // ADD:
    microcode[   0] = clean(TMP0);
    microcode[   1] = instr( IMM, TMP0);
    microcode[   2] = instr(TMP0, SRC2);
    // addi-exit:
    microcode[   3] = build(NEXT,   PC,  255);
    mapping[ SUB] = 4;
    // Instruction SUB
    // SUB:
    microcode[   4] = clean(TMP0);
    microcode[   5] = instr(SRC2,  IMM);
    microcode[   6] = instr(SRC2, SRC2);
    microcode[   7] = instr( IMM, TMP0);
    microcode[   8] = instr(TMP0, SRC2);
    microcode[   9] = build(NEXT,   PC,  255);
    mapping[AUIPC] = 10;
    // Instruction AUIPC
    // AUIPC:
    microcode[  10] = clean(TMP0);
    microcode[  11] = clean(SRC2);
    microcode[  12] = instr(  PC, TMP0);
    microcode[  13] = instr(TMP0, SRC2);
    microcode[  14] = clean(TMP0);
    microcode[  15] = instr( IMM, TMP0);
    microcode[  16] = instr(TMP0, SRC2);
    microcode[  17] = build(NEXT,   PC,  255);
    mapping[ LUI] = 18;
    // Instruction LUI
    // LUI:
    microcode[  18] = clean(TMP0);
    microcode[  19] = instr( IMM, TMP0);
    microcode[  20] = instr(TMP0, SRC2);
    microcode[  21] = build(NEXT,   PC,  255);
    // #                         JUMPS
    mapping[ JAL] = 22;
    // Instruction JAL
    // JAL:
    microcode[  22] = clean(TMP0);
    microcode[  23] = clean(TMP1);
    microcode[  24] = clean(SRC2);
    microcode[  25] = instr( IMM, TMP0);
    microcode[  26] = instr(  PC, TMP1);
    microcode[  27] = instr(TMP1, SRC2);
    microcode[  28] = instr(NEXT, SRC2);
    microcode[  29] = build(TMP0,   PC,  255);
    mapping[JALR] = 30;
    // Instruction JALR
    // JALR:
    microcode[  30] = clean(TMP0);
    microcode[  31] = clean(TMP1);
    microcode[  32] = clean(TMP2);
    microcode[  33] = clean(TMP3);
    microcode[  34] = instr(  PC, TMP0);
    microcode[  35] = instr(TMP0, TMP1);
    microcode[  36] = instr(NEXT, TMP1);
    microcode[  37] = instr(SRC2, TMP2);
    microcode[  38] = instr(TMP2,  IMM);
    microcode[  39] = clean(TMP2);
    microcode[  40] = instr( IMM, TMP2);
    microcode[  41] = clean(  PC);
    microcode[  42] = instr(TMP2,   PC);
    microcode[  43] = clean(SRC2);
    microcode[  44] = instr(TMP1, TMP3);
    microcode[  45] = build(TMP3, SRC2,  255);
    // #                         BRANCHES
    mapping[ BEQ] = 46;
    // Instruction BEQ
    // BEQ:
    microcode[  46] = clean(TMP0);
    microcode[  47] = clean(TMP1);
    microcode[  48] = instr(SRC2, TMP0);
    microcode[  49] = instr(TMP0, TMP1);
    // beq-check-one:
    microcode[  50] = build(SRC1, SRC2,    2);
    microcode[  51] = build(TMP0, TMP0,    2);
    // beq-check-two:
    microcode[  52] = build(TMP1, SRC1,    2);
    // beq-no-jump-two:
    microcode[  53] = build(NEXT,   PC,  255);
    // beq-jump:
    microcode[  54] = clean(TMP0);
    microcode[  55] = instr( IMM, TMP0);
    microcode[  56] = build(TMP0,   PC,  255);
    mapping[ BNE] = 57;
    // Instruction BNE
    // BNE:
    microcode[  57] = clean(TMP0);
    microcode[  58] = clean(TMP1);
    microcode[  59] = instr(SRC2, TMP0);
    microcode[  60] = instr(TMP0, TMP1);
    microcode[  61] = build(SRC1, SRC2,    2);
    microcode[  62] = build(TMP0, TMP0,    2);
    // bne-check-two:
    microcode[  63] = build(TMP1, SRC1,    4);
    // bne-jump:
    microcode[  64] = clean(TMP2);
    microcode[  65] = instr( IMM, TMP2);
    microcode[  66] = build(TMP2,   PC,  255);
    // bne-no-jump:
    microcode[  67] = build(NEXT,   PC,  255);
    mapping[ BLT] = 68;
    // Instruction BLT
    // BLT:
    microcode[  68] = build(SRC1, SRC2,    5);
    // blt-jump:
    microcode[  69] = clean(TMP0);
    microcode[  70] = clean(TMP1);
    microcode[  71] = instr( IMM, TMP0);
    microcode[  72] = build(TMP0,   PC,  255);
    // blt-no-jump:
    microcode[  73] = build(NEXT,   PC,  255);
    mapping[ BGE] = 74;
    // Instruction BGE
    // BGE:
    microcode[  74] = build(SRC1, SRC2,    2);
    // bge-no-jump:
    microcode[  75] = build(NEXT,   PC,  255);
    // bge-jump:
    microcode[  76] = clean(TMP0);
    microcode[  77] = instr( IMM, TMP0);
    microcode[  78] = build(TMP0,   PC,  255);
    mapping[BLTU] = 79;
    // Instruction BLTU
    // BLTU:
    microcode[  79] = clean(TMP0);
    microcode[  80] = clean(TMP3);
    microcode[  81] = clean(TMP4);
    microcode[  82] = clean(TMP5);
    microcode[  83] = instr(WORD, TMP5);
    microcode[  84] = build(TMP0, TMP0,    8);
    // bltu-loop:
    microcode[  85] = clean(TMP4);
    microcode[  86] = instr(SRC1, TMP4);
    microcode[  87] = instr(TMP4, SRC1);
    microcode[  88] = clean(TMP4);
    microcode[  89] = instr(SRC2, TMP4);
    microcode[  90] = instr(TMP4, SRC2);
    microcode[  91] = clean(TMP4);
    // bltu-check:
    microcode[  92] = clean(TMP3);
    microcode[  93] = build(SRC2, TMP3,    3);
    microcode[  94] = build(SRC1, TMP3,   10);
    microcode[  95] = build(TMP3, TMP3,    4);
    // bltu-srctwo-pos:
    microcode[  96] = clean(TMP3);
    microcode[  97] = build(SRC1, TMP3,    2);
    microcode[  98] = build(TMP0, TMP0,    5);
    // bltu-next-bit:
    microcode[  99] = clean(TMP0);
    microcode[ 100] = build(TMP5, TMP0,    3);
    microcode[ 101] = instr( INC, TMP5);
    microcode[ 102] = build(TMP0, TMP0,  -17);
    // bltu-next-instruction:
    microcode[ 103] = build(NEXT,   PC,  255);
    // bltu-jump:
    microcode[ 104] = instr( IMM, TMP0);
    microcode[ 105] = build(TMP0,   PC,  255);
    mapping[BGEU] = 106;
    // Instruction BGEU
    // BGEU:
    microcode[ 106] = clean(TMP0);
    microcode[ 107] = clean(TMP3);
    microcode[ 108] = clean(TMP4);
    microcode[ 109] = clean(TMP5);
    microcode[ 110] = instr(WORD, TMP5);
    microcode[ 111] = build(TMP0, TMP0,    8);
    // bgeu-loop:
    microcode[ 112] = clean(TMP4);
    microcode[ 113] = instr(SRC1, TMP4);
    microcode[ 114] = instr(TMP4, SRC1);
    microcode[ 115] = clean(TMP4);
    microcode[ 116] = instr(SRC2, TMP4);
    microcode[ 117] = instr(TMP4, SRC2);
    microcode[ 118] = clean(TMP4);
    // bgeu-check:
    microcode[ 119] = clean(TMP3);
    microcode[ 120] = build(SRC2, TMP3,    4);
    microcode[ 121] = clean(TMP3);
    microcode[ 122] = build(SRC1, TMP3,   10);
    microcode[ 123] = build(TMP3, TMP3,    4);
    // bgeu-srctwo-pos:
    microcode[ 124] = clean(TMP3);
    microcode[ 125] = build(SRC1, TMP3,    2);
    microcode[ 126] = build(TMP0, TMP0,    5);
    // bgeu-next-bit:
    microcode[ 127] = clean(TMP0);
    microcode[ 128] = build(TMP5, TMP0,    4);
    microcode[ 129] = instr( INC, TMP5);
    microcode[ 130] = build(TMP0, TMP0,  -18);
    // bgeu-next-instruction:
    microcode[ 131] = build(NEXT,   PC,  255);
    // bgeu-jump:
    microcode[ 132] = instr( IMM, TMP0);
    microcode[ 133] = build(TMP0,   PC,  255);
    // #                         BIT LOGIC
    mapping[XORI] = 134;
    // Instruction XORI
    // XORI:
    mapping[ XOR] = 134;
    // Instruction XOR
    // XOR:
    microcode[ 134] = clean(TMP0);
    microcode[ 135] = clean(TMP1);
    microcode[ 136] = clean(TMP2);
    microcode[ 137] = clean(TMP3);
    microcode[ 138] = clean(TMP4);
    microcode[ 139] = clean(TMP5);
    microcode[ 140] = clean(SRC1);
    microcode[ 141] = instr(SRC2, TMP0);
    microcode[ 142] = instr(TMP0, SRC1);
    microcode[ 143] = clean(RSLT);
    microcode[ 144] = clean(TMP0);
    microcode[ 145] = instr(WORD, TMP5);
    microcode[ 146] = instr( ONE, TMP4);
    // xor-loop:
    microcode[ 147] = instr(RSLT, TMP3);
    microcode[ 148] = instr(TMP3, RSLT);
    microcode[ 149] = clean(TMP3);
    // xor-get-msb-src-one:
    microcode[ 150] = clean(TMP0);
    microcode[ 151] = build(SRC1, TMP0,    2);
    microcode[ 152] = instr( INC, TMP1);
    // xor-get-msb-src-two:
    microcode[ 153] = clean(TMP0);
    microcode[ 154] = clean(TMP2);
    microcode[ 155] = build( IMM, TMP0,    2);
    microcode[ 156] = instr( INC, TMP2);
    // xor-get-msb-clean:
    microcode[ 157] = clean(TMP0);
    // xor-add:
    microcode[ 158] = instr(TMP1, TMP3);
    microcode[ 159] = instr(TMP3, TMP2);
    microcode[ 160] = clean(TMP3);
    // xor-check-one:
    microcode[ 161] = build( ONE, TMP2,    2);
    microcode[ 162] = build(TMP3, TMP3,    7);
    // xor-check-two:
    microcode[ 163] = instr( INC, TMP2);
    microcode[ 164] = clean(TMP3);
    microcode[ 165] = instr(TMP4, TMP3);
    microcode[ 166] = build(TMP2, TMP3,    2);
    microcode[ 167] = build(TMP3, TMP3,    2);
    // xor-set-bit:
    microcode[ 168] = instr( INC, RSLT);
    // xor-shift:
    microcode[ 169] = instr(SRC1, TMP3);
    microcode[ 170] = instr(TMP3, SRC1);
    microcode[ 171] = clean(TMP3);
    microcode[ 172] = instr( IMM, TMP3);
    microcode[ 173] = instr(TMP3,  IMM);
    microcode[ 174] = clean(TMP3);
    microcode[ 175] = build( INC, TMP5,  -28);
    // xor-clean:
    microcode[ 176] = build(NEXT,   PC,  255);
    mapping[ ORI] = 177;
    // Instruction ORI
    // ORI:
    mapping[  OR] = 177;
    // Instruction OR
    // OR:
    microcode[ 177] = clean(TMP0);
    microcode[ 178] = clean(TMP5);
    microcode[ 179] = clean(SRC1);
    microcode[ 180] = instr(SRC2, TMP0);
    microcode[ 181] = instr(TMP0, SRC1);
    microcode[ 182] = clean(RSLT);
    microcode[ 183] = instr(WORD, TMP5);
    // or-loop:
    microcode[ 184] = clean(TMP0);
    microcode[ 185] = instr(RSLT, TMP0);
    microcode[ 186] = instr(TMP0, RSLT);
    // or-get-msb-src-one:
    microcode[ 187] = clean(TMP0);
    microcode[ 188] = build(SRC1, TMP0,    2);
    microcode[ 189] = build(TMP0, TMP0,    3);
    // or-get-msb-src-two:
    microcode[ 190] = clean(TMP0);
    microcode[ 191] = build( IMM, TMP0,    2);
    // or-set-bit:
    microcode[ 192] = instr( INC, RSLT);
    // or-shift:
    microcode[ 193] = clean(TMP0);
    microcode[ 194] = instr(SRC1, TMP0);
    microcode[ 195] = instr(TMP0, SRC1);
    microcode[ 196] = clean(TMP0);
    microcode[ 197] = instr( IMM, TMP0);
    microcode[ 198] = instr(TMP0,  IMM);
    microcode[ 199] = clean(TMP0);
    microcode[ 200] = build( INC, TMP5,  -16);
    // or-clean:
    microcode[ 201] = build(NEXT,   PC,  255);
    mapping[ANDI] = 202;
    // Instruction ANDI
    // ANDI:
    mapping[ AND] = 202;
    // Instruction AND
    // AND:
    microcode[ 202] = clean(TMP0);
    microcode[ 203] = clean(TMP3);
    microcode[ 204] = clean(TMP5);
    microcode[ 205] = clean(SRC1);
    microcode[ 206] = instr(SRC2, TMP0);
    microcode[ 207] = instr(TMP0, SRC1);
    microcode[ 208] = clean(RSLT);
    microcode[ 209] = instr(WORD, TMP5);
    // and-loop:
    microcode[ 210] = clean(TMP3);
    microcode[ 211] = instr(RSLT, TMP3);
    microcode[ 212] = instr(TMP3, RSLT);
    // and-get-msb-src-one:
    microcode[ 213] = clean(TMP0);
    microcode[ 214] = build(SRC1, TMP0,    4);
    // and-get-msb-src-two:
    microcode[ 215] = clean(TMP0);
    microcode[ 216] = build( IMM, TMP0,    2);
    // and-set-bit:
    microcode[ 217] = instr( INC, RSLT);
    // and-shift:
    microcode[ 218] = clean(TMP3);
    microcode[ 219] = instr(SRC1, TMP3);
    microcode[ 220] = instr(TMP3, SRC1);
    microcode[ 221] = clean(TMP3);
    microcode[ 222] = instr( IMM, TMP3);
    microcode[ 223] = instr(TMP3,  IMM);
    microcode[ 224] = build( INC, TMP5,  -14);
    // and-clean:
    microcode[ 225] = build(NEXT,   PC,  255);
    // #                         SHIFTING
    mapping[SLLI] = 226;
    // Instruction SLLI
    // SLLI:
    mapping[ SLL] = 226;
    // Instruction SLL
    // SLL:
    microcode[ 226] = clean(TMP0);
    microcode[ 227] = clean(TMP1);
    microcode[ 228] = build(SRC1, TMP0,    2);
    // sll-loop:
    microcode[ 229] = instr(SRC2, TMP1);
    // sll-shift:
    microcode[ 230] = instr(TMP1, SRC2);
    microcode[ 231] = clean(TMP1);
    microcode[ 232] = build( INC, TMP0,   -3);
    // sll-clean:
    microcode[ 233] = build(NEXT,   PC,  255);
    mapping[SRAI] = 234;
    // Instruction SRAI
    // SRAI:
    mapping[ SRA] = 234;
    // Instruction SRA
    // SRA:
    microcode[ 234] = clean(TMP0);
    microcode[ 235] = build(SRC1, TMP0,    4);
    // sra-is-neg:
    microcode[ 236] = clean(TMP0);
    microcode[ 237] = instr( INC, TMP0);
    microcode[ 238] = instr(TMP0, RSLT);
    // sra-is-pos:
    mapping[SRLI] = 239;
    // Instruction SRLI
    // SRLI:
    mapping[ SRL] = 239;
    // Instruction SRL
    // SRL:
    // srl:
    microcode[ 239] = clean(TMP0);
    microcode[ 240] = clean(TMP5);
    microcode[ 241] = instr(WORD, TMP5);
    microcode[ 242] = instr( IMM, TMP0);
    microcode[ 243] = instr(TMP0, TMP5);
    // srl-msb-check:
    microcode[ 244] = clean(TMP0);
    microcode[ 245] = build(SRC1, TMP0,    6);
    // srl-msb-set:
    microcode[ 246] = clean(TMP0);
    microcode[ 247] = instr(RSLT, TMP0);
    microcode[ 248] = instr(TMP0, RSLT);
    microcode[ 249] = instr( INC, RSLT);
    microcode[ 250] = build(TMP0, TMP0,    4);
    // srl-msb-unset:
    microcode[ 251] = clean(TMP0);
    microcode[ 252] = instr(RSLT, TMP0);
    microcode[ 253] = instr(TMP0, RSLT);
    // srl-check-loop:
    microcode[ 254] = clean(TMP0);
    microcode[ 255] = build(TMP5, TMP0,    6);
    // srl-move-op:
    microcode[ 256] = clean(TMP0);
    microcode[ 257] = instr(SRC1, TMP0);
    microcode[ 258] = instr(TMP0, SRC1);
    microcode[ 259] = instr( INC, TMP5);
    microcode[ 260] = build(TMP0, TMP0,  -16);
    // srl-end:
    microcode[ 261] = build(NEXT,   PC,  255);
    // #                         SET BIT
    mapping[SLTI] = 262;
    // Instruction SLTI
    // SLTI:
    mapping[ SLT] = 262;
    // Instruction SLT
    // SLT:
    // slt-check-one:
    microcode[ 262] = build(SRC1, SRC2,    4);
    // slt-set:
    microcode[ 263] = clean(SRC2);
    microcode[ 264] = instr( INC, SRC2);
    microcode[ 265] = build(NEXT,   PC,  255);
    // slt-no-set:
    microcode[ 266] = clean(SRC2);
    microcode[ 267] = build(NEXT,   PC,  255);
    mapping[SLTIU] = 268;
    // Instruction SLTIU
    // SLTIU:
    mapping[SLTU] = 268;
    // Instruction SLTU
    // SLTU:
    // # jump if src1 < src2 unsigned
    microcode[ 268] = clean(TMP0);
    microcode[ 269] = clean(TMP3);
    microcode[ 270] = clean(TMP4);
    microcode[ 271] = clean(TMP5);
    microcode[ 272] = instr(WORD, TMP5);
    microcode[ 273] = build(TMP0, TMP0,    8);
    // sltu-loop:
    microcode[ 274] = clean(TMP4);
    microcode[ 275] = instr(SRC1, TMP4);
    microcode[ 276] = instr(TMP4, SRC1);
    microcode[ 277] = clean(TMP4);
    microcode[ 278] = instr(SRC2, TMP4);
    microcode[ 279] = instr(TMP4, SRC2);
    microcode[ 280] = clean(TMP4);
    // sltu-check:
    microcode[ 281] = clean(TMP3);
    microcode[ 282] = build(SRC2, TMP3,    4);
    microcode[ 283] = clean(TMP3);
    microcode[ 284] = build(SRC1, TMP3,   10);
    microcode[ 285] = build(TMP3, TMP3,    4);
    // sltu-srctwo-pos:
    microcode[ 286] = clean(TMP3);
    microcode[ 287] = build(SRC1, TMP3,    2);
    microcode[ 288] = build(TMP0, TMP0,    5);
    // sltu-next-bit:
    microcode[ 289] = clean(TMP0);
    microcode[ 290] = build(TMP5, TMP0,    3);
    microcode[ 291] = instr( INC, TMP5);
    microcode[ 292] = build(TMP0, TMP0,  -18);
    // sltu-next-instruction:
    microcode[ 293] = build(RSLT, RSLT,    3);
    // sltu-jump:
    microcode[ 294] = clean(RSLT);
    microcode[ 295] = instr( INC, RSLT);
    // sltu-exit:
    microcode[ 296] = build(NEXT,   PC,  255);
  }

  int32_t func(uint32_t address, int32_t value) {
    return value;
  }

};
