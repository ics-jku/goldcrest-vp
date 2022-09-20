#pragma once

#include "core.hpp"

#define RXOR FUNC1
#define RAND FUNC2
#define ROR  FUNC3
#define RACC FUNC4

class BitopsCore : GoldcrestCore {
public:
  BitopsCore(Memory *memory): GoldcrestCore(memory) {
    init();
  }

  void init() {
    mapping[ADDI] = 0;
    mapping[ADD] = 0;
    mapping[SUB] = 4;
    mapping[JAL] = 9;
    mapping[JALR] = 21;
    mapping[LB] = 17;
    mapping[LBU] = 17;  
    mapping[LH] = 17;
    mapping[LHU] = 17;  
    mapping[LW] = 17;
    mapping[SB] = 17;
    mapping[SH] = 17;
    mapping[SW] = 17;
    mapping[BLT] = 450;
    mapping[AUIPC] = 60;
    mapping[LUI] = 70;
    mapping[BEQ] = 75;
    mapping[BNE] = 462; //90;
    mapping[SLL] = 110;
    mapping[SLLI] = 110;
    mapping[BGEU] = 267;
    mapping[BLTU] = 310;
    mapping[BGE] = 456; // 360
    mapping[SRL] = 377;
    mapping[SRLI] = 377;
    mapping[SLT] = 475;
    mapping[SLTI] = 475;    
    
    // subleq a b l: mem[b] = mem[b] - mem[a] => if(mem[b] <= 0) { pc + l } else { pc + 1 }
    // ADDI r, d, i | ADD
    microcode[0]  = build(IMM , TMP0, 1);     // negate IMM   | TMP0 - IMM = 0 - mem[IMM]
    microcode[1]  = build(TMP0, SRC2, 1);     // SRC2 - -(TMP0)
    microcode[2]  = build(TMP0, TMP0, 1);     // reset TMP0 and exit
    microcode[3]  = build(NEXT, PC, END);     // increment RISC-V pc
    // SUB
    microcode[4]  = build(SRC2, IMM, 1);    // SRC2 - -(TMP0)
    microcode[5]  = build(SRC2, SRC2, 1);
    microcode[6]  = build(IMM, TMP0, 1);
    microcode[7]  = build(TMP0, SRC2, 1);
    microcode[8]  = build(TMP0, TMP0, -5);  

    // JAL
    microcode[9]  = build(SRC2, SRC2, 1);     // reset SRC2
    microcode[10]  = build(IMM , TMP0, 1);     // negate IMM
    microcode[11] = build(PC  , TMP1, 1);     // -pc => TMP1
    microcode[12] = build(TMP1, SRC2, 1);     // pc => SRC2
    microcode[13] = build(NEXT, SRC2, 1);       // add link address +4
    microcode[14] = build(TMP0, PC, 1);       // PC - -TMP0
    microcode[15] = build(TMP0, TMP0, 1);     // reset TMP0 and exit
    microcode[16] = build(TMP1, TMP1, END);   // reset TMP1 and exit
    // LW/SW
    microcode[17] = build(IMM , TMP0, 1);     // negate IMM
    microcode[18] = build(TMP0, SRC2, 1);     // SRC2 - -(IMM)
    microcode[19] = build(TMP0, TMP0, 1);     // reset TMP0 and exit
    microcode[20] = build(NEXT, PC, END);     // increment RISC-V pc
    // JALR
    microcode[21] = build(PC, TMP0, 1);       // -PC store link pc
    microcode[22] = build(TMP0, TMP1, 1);     // --PC
    microcode[23] = build(NEXT, TMP1, 1);     // PC + 4
    microcode[24] = build(SRC2, TMP2, 1);     // -SRC2 => TMP2   calculate jump target
    microcode[25] = build(TMP2, IMM, 1);      // IMM --(SRC2)  
    microcode[26] = build(TMP2, TMP2, 1);     // reset TMP2    set new pc after jump
    microcode[27] = build(IMM, TMP2, 1);      // -(SRC2 + IMM) => TMP2
    microcode[28] = build(PC, PC, 1);         // reset PC
    microcode[29] = build(TMP2, PC, 1);       // (SRC2 + IMM) => PC
    microcode[30] = build(SRC2, SRC2, 1);     // reset SRC2
    microcode[31] = build(TMP1, TMP3, 1);     // -(PC + 4) => TMP3
    microcode[32] = build(TMP3, SRC2, 1);     // (PC + 4) => SRC2
    microcode[33] = build(TMP0, TMP0, 1);     // reset TMP0
    microcode[34] = build(TMP1, TMP1, 1);     // reset TMP1
    microcode[35] = build(TMP2, TMP2, 1);     // reset TMP2
    microcode[36] = build(TMP3, TMP3, END);   // reset TMP3
    
    // AUIPC R[rd] = PC + U_imm
    microcode[60] = build(PC, TMP0, 1);       // copy PC to SRC2
    microcode[61] = build(TMP0, SRC2, 1);
    microcode[62] = build(TMP0, TMP0, 1);     // PC + IMM
    microcode[63] = build(IMM, TMP0, 1);
    microcode[64] = build(TMP0, SRC2, 1);
    microcode[65] = build(TMP0, TMP0, 1);
    microcode[66] = build(NEXT, PC, END);
    // LUI R[rd] = U_imm
    microcode[70] = build(IMM, TMP0, 1);
    microcode[71] = build(TMP0, SRC2, 11);
    microcode[72] = build(TMP0, TMP0, 1);
    microcode[73] = build(NEXT, PC, END);
    // BEQ
    microcode[75] = build(SRC2, TMP0, 1);    // copy SRC2 to TMP1
    microcode[76] = build(TMP0, TMP1, 1);
    microcode[77] = build(SRC1, SRC2, 4);    // test if SRC2 <= SRC1
    microcode[78] = build(TMP0, TMP0, 1);    // NO
    microcode[79] = build(TMP1, TMP1, 1);    // clean
    microcode[80] = build(NEXT, PC, END);    // goto next instruction
    microcode[81] = build(TMP1, SRC1, 4);    // test if SRC1 <= SRC2
    microcode[82] = build(TMP0, TMP0, 1);    // NO
    microcode[83] = build(TMP1, TMP1, 1);    // clean
    microcode[84] = build(NEXT, PC, END);    // goto next instruction
    microcode[85] = build(IMM, TMP2, 1);       // YES
    microcode[86] = build(TMP2, PC, 1);        // pc + B_imm
    microcode[87] = build(TMP2, TMP2, 1);        // pc + B_imm  
    microcode[88] = build(TMP0, TMP0, 1);      // clear and exit
    microcode[89] = build(TMP1, TMP1, END);
    // SLL SRC2 = SRC1; SRC1 = SHIFT AMMOUNT
    microcode[110] = build(SRC1, TMP0, 2);     // -SRC2 -> TMP0
    microcode[111] = build(SRC2, TMP1, 1);     // negate shift value
    microcode[112] = build(TMP1, SRC2, 1);     // SRC2 + SRC2
    microcode[113] = build(TMP1, TMP1, 1);     // reset TMP1
    microcode[114] = build(INC, TMP0, -3);     // loop
    microcode[115] = build(TMP0, TMP0, 1);     // clean
    microcode[116] = build(TMP1, TMP1, 1);     // clean
    microcode[117] = build(NEXT, PC, END);     // next RISC-V

    // BGEU
    microcode[267] = instr(WORD, TMP5);
    // loop
    microcode[268] = clean(TMP0);
    microcode[269] = clean(TMP1);
    microcode[270] = clean(TMP2);    
    microcode[271] = instr(SRC1, TMP0);
    microcode[272] = instr(TMP0, MSB);
    microcode[273] = instr(MSB, TMP1);
    microcode[274] = clean(TMP0);
    microcode[275] = instr(SRC2, TMP0);
    microcode[276] = instr(TMP0, MSB);
    microcode[277] = instr(MSB, TMP2);
    microcode[278] = clean(TMP0);
    // compare
    microcode[279] = instr(TMP1, TMP4);
    microcode[280] = instr(TMP4, TMP3);
    microcode[281] = clean(TMP4);
    microcode[282] = instr(INC, TMP1);
    microcode[283] = build(TMP2, TMP1, (301-283)); // 0 = nojump// (293-279)); // when TMP1 - TMP2 <= 0 then TMP1 == 1 and TMP2 == 0
    microcode[284] = instr(INC, TMP2);
    microcode[285] = build(TMP3, TMP2, (293-285)); // 0 = jump
    // next: shift SRC1 and SRC2
    microcode[286] = clean(TMP3);
    microcode[287] = instr(SRC1, TMP0);
    microcode[288] = instr(TMP0, SRC1);
    microcode[289] = clean(TMP0);
    microcode[290] = instr(SRC2, TMP0);
    microcode[291] = instr(TMP0, SRC2);    
    // check and loop back
    microcode[292] = build(INC, TMP5, (268-292));
    // jump
    microcode[293] = clean(TMP0);
    microcode[294] = instr(IMM, TMP0);
    microcode[295] = instr(TMP0, PC);
    microcode[296] = clean(TMP0);
    microcode[297] = clean(TMP1);
    microcode[298] = clean(TMP2);
    microcode[299] = clean(TMP3);
    microcode[300] = build(TMP5, TMP5, END);
    // no jump
    microcode[301] = clean(TMP0);
    microcode[302] = clean(TMP1);
    microcode[303] = clean(TMP2);
    microcode[304] = clean(TMP3);
    microcode[305] = clean(TMP5);
    microcode[306] = build(NEXT, PC, END);

    // BLTU
    microcode[310] = instr(WORD, TMP5);
    // loop
    microcode[311] = clean(TMP0);
    microcode[312] = clean(TMP1);
    microcode[313] = clean(TMP2);    
    microcode[314] = instr(SRC1, TMP0);
    microcode[315] = instr(TMP0, MSB);
    microcode[316] = instr(MSB, TMP1);
    microcode[317] = clean(TMP0);
    microcode[318] = instr(SRC2, TMP0);
    microcode[319] = instr(TMP0, MSB);
    microcode[320] = instr(MSB, TMP2);
    microcode[321] = clean(TMP0);
    // compare
    microcode[322] = instr(TMP1, TMP4);
    microcode[323] = instr(TMP4, TMP3);
    microcode[324] = clean(TMP4);
    microcode[325] = instr(INC, TMP1);
    microcode[326] = build(TMP2, TMP1, (336-326)); // 0 = jump// (293-279)); // when TMP1 - TMP2 <= 0 then TMP1 == 1 and TMP2 == 0
    microcode[327] = instr(INC, TMP2);
    microcode[328] = build(TMP3, TMP2, (344-328)); // 0 = nojump
    // next: shift SRC1 and SRC2
    microcode[329] = clean(TMP3);
    microcode[330] = instr(SRC1, TMP0);
    microcode[331] = instr(TMP0, SRC1);
    microcode[332] = clean(TMP0);
    microcode[333] = instr(SRC2, TMP0);
    microcode[334] = instr(TMP0, SRC2);    
    // check and loop back
    microcode[335] = build(INC, TMP5, (311-335));
    // jump
    microcode[336] = clean(TMP0);
    microcode[337] = instr(IMM, TMP0);
    microcode[338] = instr(TMP0, PC);
    microcode[339] = clean(TMP0);
    microcode[340] = clean(TMP1);
    microcode[341] = clean(TMP2);
    microcode[342] = clean(TMP3);
    microcode[343] = build(TMP5, TMP5, END);
    // no jump
    microcode[344] = clean(TMP0);
    microcode[345] = clean(TMP1);
    microcode[346] = clean(TMP2);
    microcode[347] = clean(TMP3);
    microcode[348] = clean(TMP5);
    microcode[349] = build(NEXT, PC, END);    

    // SRLI SRC2=reg1, SRC1=imm/reg2
    microcode[377] = build(TMP0, SRC1, 13); // if SRC1 == 0 exit
    microcode[378] = instr(SRC1, TMP5);
    microcode[379] = instr(INC, TMP5);
    // >>1
    microcode[380] = instr(INC, SRC2);
    microcode[381] = build(TWO, SRC2, 3); 
    // loop
    microcode[382] = instr(INC, TMP0);
    microcode[383] = build(TMP1, TMP1, -2);
    // end inner
    microcode[384] = clean(SRC2);
    microcode[385] = instr(TMP0, TMP1);
    microcode[386] = instr(TMP1, SRC2);
    microcode[387] = clean(TMP0);
    microcode[388] = clean(TMP1);
    microcode[389] = build(INC, TMP5, -9);
    // end
    microcode[390] = clean(TMP5);
    microcode[391] = build(NEXT, PC, END);

    // BLT really fast
    // check src1 <= src2
    microcode[450] = build(SRC1, SRC2, 5); // no jump src1 >= src2
    // src1 < src2, jump
    microcode[451] = instr(IMM, TMP0);
    microcode[452] = instr(TMP0, PC);
    microcode[453] = clean(TMP0);
    microcode[454] = build(TMP2, TMP2, END);
    // no jump
    microcode[455] = build(NEXT, PC, END);

    // BGE really fast
    // check src1 <= src2
    microcode[456] = build(SRC1, SRC2, 2); // no jump src1 >= src2
    // no jump
    microcode[457] = build(NEXT, PC, END);    
    // src1 < src2, jump
    microcode[458] = instr(IMM, TMP0);
    microcode[459] = instr(TMP0, PC);
    microcode[460] = clean(TMP0);
    microcode[461] = build(TMP2, TMP2, END);
    
    // BNE correct
    microcode[462] = instr(SRC2, TMP0);       // copy SRC2 to TMP1
    microcode[463] = instr(TMP0, TMP1);
    microcode[464] = build(SRC1, SRC2, 2);    // test if SRC2 <= SRC1
    // goto jump
    microcode[465] = build(TMP0, TMP0, 2);
    microcode[466] = build(TMP1, SRC1, 6);    // test if SRC1 <= SRC2
    // jump
    microcode[467] = instr(IMM, TMP2);        // YES
    microcode[468] = instr(TMP2, PC);         // pc + B_imm
    microcode[469] = clean(TMP2);             // pc + B_imm  
    microcode[470] = clean(TMP0);             // clear and exit
    microcode[471] = build(TMP1, TMP1, END);
    // no jump
    microcode[472] = instr(TMP0, TMP0);       
    microcode[473] = clean(TMP1);
    microcode[474] = build(NEXT, PC, END);

    // SLT really fast
    // check src1 <= src2
    microcode[475] = build(SRC1, SRC2, 4); // no jump src1 >= src2
    // src1 < src2, jump
    microcode[476] = clean(SRC2);
    microcode[477] = instr(INC, SRC2);
    microcode[478] = build(NEXT, PC, END);
    // no jump
    microcode[479] = clean(SRC2);
    microcode[480] = build(NEXT, PC, END);


    // --------------------------------------------------------------------------------

    // Instruction BLTU
    mapping[BLTU] = 481;
    //  jump if src1 < src2 unsigned
    // bltu:
    microcode[481] = instr(WORD, TMP5);
    microcode[482] = build(TMP0, TMP0, 8);
    // bltu-loop:
    microcode[483] = clean(TMP4);
    microcode[484] = instr(SRC1, TMP4);
    microcode[485] = instr(TMP4, SRC1);
    microcode[486] = clean(TMP4);
    microcode[487] = instr(SRC2, TMP4);
    microcode[488] = instr(TMP4, SRC2);
    microcode[489] = clean(TMP4);
    // bltu-check:
    microcode[490] = clean(TMP3);
    microcode[491] = build(SRC2, TMP3, 4);
    microcode[492] = clean(TMP3);
    microcode[493] = build(SRC1, TMP3, 14);
    microcode[494] = build(TMP3, TMP3, 4);
    // bltu-srctwo-pos:
    microcode[495] = clean(TMP3);
    microcode[496] = build(SRC1, TMP3, 2);
    microcode[497] = build(TMP0, TMP0, 5);
    // bltu-next-bit:
    microcode[498] = clean(TMP0);
    microcode[499] = build(TMP5, TMP0, 3);
    microcode[500] = instr(INC, TMP5);
    microcode[501] = build(TMP0, TMP0, -18);
    // bltu-next-instruction:
    microcode[502] = clean(TMP0);
    microcode[503] = clean(TMP3);
    microcode[504] = clean(TMP4);
    microcode[505] = clean(TMP5);
    microcode[506] = build(NEXT, PC, END);
    // bltu-jump:
    microcode[507] = clean(TMP0);
    microcode[508] = clean(TMP3);
    microcode[509] = clean(TMP4);
    microcode[510] = clean(TMP5);
    microcode[511] = instr(IMM, TMP0);
    microcode[512] = instr(TMP0, PC);
    microcode[513] = build(TMP0, TMP0, END);
    // Instruction SLTIU
    mapping[SLTIU] = 514;
    // Instruction SLTU
    mapping[SLTU] = 514;
    //  jump if src1 < src2 unsigned
    // sltu:
    microcode[514] = instr(WORD, TMP5);
    microcode[515] = build(TMP0, TMP0, 8);
    // sltu-loop:
    microcode[516] = clean(TMP4);
    microcode[517] = instr(SRC1, TMP4);
    microcode[518] = instr(TMP4, SRC1);
    microcode[519] = clean(TMP4);
    microcode[520] = instr(SRC2, TMP4);
    microcode[521] = instr(TMP4, SRC2);
    microcode[522] = clean(TMP4);
    // sltu-check:
    microcode[523] = clean(TMP3);
    microcode[524] = build(SRC2, TMP3, 4);
    microcode[525] = clean(TMP3);
    microcode[526] = build(SRC1, TMP3, 10);
    microcode[527] = build(TMP3, TMP3, 4);
    // sltu-srctwo-pos:
    microcode[528] = clean(TMP3);
    microcode[529] = build(SRC1, TMP3, 2);
    microcode[530] = build(TMP0, TMP0, 5);
    // sltu-next-bit:
    microcode[531] = clean(TMP0);
    microcode[532] = build(TMP5, TMP0, 3);
    microcode[533] = instr(INC, TMP5);
    microcode[534] = build(TMP0, TMP0, -18);
    // sltu-next-instruction:
    microcode[535] = build(RSLT, RSLT, 3);
    // sltu-jump:
    microcode[536] = clean(RSLT);
    microcode[537] = instr(INC, RSLT);
    // sltu-exit:
    microcode[538] = clean(TMP0);
    microcode[539] = clean(TMP3);
    microcode[540] = clean(TMP4);
    microcode[541] = clean(TMP5);
    microcode[542] = build(NEXT, PC, END);
    // Instruction BGEU
    mapping[BGEU] = 543;
    //  jump if src1 < src2 unsigned
    // bgeu:
    microcode[543] = instr(WORD, TMP5);
    microcode[544] = build(TMP0, TMP0, 8);
    // bgeu-loop:
    microcode[545] = clean(TMP4);
    microcode[546] = instr(SRC1, TMP4);
    microcode[547] = instr(TMP4, SRC1);
    microcode[548] = clean(TMP4);
    microcode[549] = instr(SRC2, TMP4);
    microcode[550] = instr(TMP4, SRC2);
    microcode[551] = clean(TMP4);
    // bgeu-check:
    microcode[552] = clean(TMP3);
    microcode[553] = build(SRC2, TMP3, 4);
    microcode[554] = clean(TMP3);
    microcode[555] = build(SRC1, TMP3, 14);
    microcode[556] = build(TMP3, TMP3, 4);
    // bgeu-srctwo-pos:
    microcode[557] = clean(TMP3);
    microcode[558] = build(SRC1, TMP3, 2);
    microcode[559] = build(TMP0, TMP0, 5);
    // bgeu-next-bit:
    microcode[560] = clean(TMP0);
    microcode[561] = build(TMP5, TMP0, 8);
    microcode[562] = instr(INC, TMP5);
    microcode[563] = build(TMP0, TMP0, -18);
    // bgeu-next-instruction:
    microcode[564] = clean(TMP0);
    microcode[565] = clean(TMP3);
    microcode[566] = clean(TMP4);
    microcode[567] = clean(TMP5);
    microcode[568] = build(NEXT, PC, END);
    // bgeu-jump:
    microcode[569] = clean(TMP0);
    microcode[570] = clean(TMP3);
    microcode[571] = clean(TMP4);
    microcode[572] = clean(TMP5);
    microcode[573] = instr(IMM, TMP0);
    microcode[574] = instr(TMP0, PC);
    microcode[575] = build(TMP0, TMP0, END);
    
    // XOR fast
    mapping[XOR] = 580;
    microcode[580] = clean(RXOR);
    microcode[581] = instr(SRC2, TMP0);
    microcode[582] = instr(TMP0, RACC);
    microcode[583] = clean(TMP0);
    microcode[584] = instr(IMM, TMP0);
    microcode[585] = instr(TMP0, RXOR);
    microcode[586] = clean(TMP0);
    microcode[587] = clean(SRC2);
    microcode[588] = clean(RACC);
    microcode[589] = instr(RXOR, TMP0);
    microcode[590] = instr(TMP0, RSLT);
    microcode[591] = clean(TMP0);
    microcode[592] = build(NEXT, PC, END);

    // AND fast ich habe gerade die schnellen bitops impllementiert um vergleich in tabelle anzustellen, noch nicht getestetxmagit
    mapping[AND] = 593;
    microcode[593] = clean(RAND);
    microcode[594] = instr(SRC2, TMP0);
    microcode[595] = instr(TMP0, RACC);
    microcode[596] = clean(TMP0);
    microcode[597] = instr(IMM, TMP0);
    microcode[598] = instr(TMP0, RAND);
    microcode[599] = clean(TMP0);
    microcode[600] = clean(SRC2);
    microcode[601] = clean(RACC);
    microcode[602] = instr(RAND, TMP0);
    microcode[603] = instr(TMP0, RSLT);
    microcode[604] = clean(TMP0);
    microcode[605] = build(NEXT, PC, END);

    // OR fast
    mapping[OR] = 606;
    microcode[606] = clean(ROR);
    microcode[607] = instr(SRC2, TMP0);
    microcode[608] = instr(TMP0, RACC);
    microcode[609] = clean(TMP0);
    microcode[610] = instr(IMM, TMP0);
    microcode[611] = instr(TMP0, ROR);
    microcode[612] = clean(TMP0);
    microcode[613] = clean(SRC2);
    microcode[614] = clean(RACC);
    microcode[615] = instr(ROR, TMP0);
    microcode[616] = instr(TMP0, RSLT);
    microcode[617] = clean(TMP0);
    microcode[618] = build(NEXT, PC, END);    
    
  }

  int32_t acc = 0;
  int32_t func(uint32_t address, int32_t value) {
    auto res = value;
    if (address == RXOR) {
      res = value ^ acc;
    } else if (address == RAND) {
      res = value & acc;
    } else if (address == ROR) {
      res = value | acc;      
    } else if (address == RACC) {
      acc = value;
      return 0;
    }

    return res;
  }

};
