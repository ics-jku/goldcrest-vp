! Bitops
# ============================================================
#                         ARITHMETIC
# ============================================================
@LB
@LH
@LW
@LBU
@LHU
@SB
@SH
@SW
@ADDI
@ADD
	IMM  TMP0		# negate IMM   | TMP0 - IMM = 0 - mem[IMM]
	TMP0 SRC2     	  	# SRC2 - -(TMP0)
	TMP0 	      	  	# reset TMP0 and exit
addi-exit:	
	NEXT PC END		# increment RISC-V pc

@SUB
	SRC2 IMM		# SRC2 - -(TMP0)
	SRC2 SRC2
	IMM TMP0
	TMP0 SRC2
	TMP0 TMP0 addi-exit

@AUIPC
	PC TMP0			# copy PC to SRC2
	TMP0 SRC2
	TMP0			# PC + IMM
	IMM TMP0
	TMP0 SRC2
	TMP0
	NEXT PC END

@LUI
	IMM TMP0
	TMP0 SRC2
	TMP0
	NEXT PC END

# ============================================================
#                         JUMPS
# ============================================================
@JAL
	SRC2			# reset SRC2
	IMM  TMP0	      	# negate IMM
	PC   TMP1     	       	# -pc => TMP1
	TMP1 SRC2     	       	# pc => SRC2
	NEXT SRC2              	# add link address +4
	TMP0 PC       	       	# PC - -TMP0
	TMP0          	       	# reset TMP0 and exit
	TMP1 TMP1 END          	# reset TMP1 and exit

@JALR
	PC TMP0			# -PC store link pc
	TMP0 TMP1     		# --PC
	NEXT TMP1     		# PC + 4
	SRC2 TMP2     		# -SRC2 => TMP2   calculate jump target
	TMP2 IMM      		# IMM --(SRC2)  
	TMP2     		# reset TMP2    set new pc after jump
	IMM TMP2      		# -(SRC2 + IMM) => TMP2
	PC         		# reset PC
	TMP2 PC       		# (SRC2 + IMM) => PC
	SRC2     		# reset SRC2
	TMP1 TMP3     		# -(PC + 4) => TMP3
	TMP3 SRC2     		# (PC + 4) => SRC2
	TMP0     		# reset TMP0
	TMP1     		# reset TMP1
	TMP2     		# reset TMP2
	TMP3 TMP3 END		# reset TMP3

# ============================================================
#                         BRANCHES
# ============================================================
@BEQ
	SRC2 TMP0		# copy SRC2 to TMP1
	TMP0 TMP1
beq-check-one:
	SRC1 SRC2 beq-check-two
beq-no-jump-two:	
	TMP0 TMP0 beq-no-jump-two # NO
beq-check-two:	
	TMP1 SRC1 beq-jump	# test if SRC1 <= SRC2
beq-no-jump-two:
	TMP0    		# NO
	TMP1    		# clean
	NEXT PC END    		# goto next instruction
beq-jump:
	TMP0
	IMM TMP0       		# YES
	TMP0 PC        		# pc + B_imm
	TMP0     		# clear and exit
	TMP1 TMP1 END

@BNE
	SRC2 TMP0		# copy SRC2 to TMP1
	TMP0 TMP1
	SRC1 SRC2 bne-check-two	# test if SRC2 <= SRC1
	TMP0 TMP0 bne-jump
bne-check-two:
	TMP1 SRC1 bne-no-jump   # test if SRC1 <= SRC2
bne-jump:
	IMM TMP2
	TMP2 PC
	TMP2
	TMP0             # clear and exit
	TMP1 TMP1 END
bne-no-jump:	
	TMP0
	TMP1
	NEXT PC END

@BLT
	# check src1 <= src2
	SRC1 SRC2 blt-no-jump
blt-jump:
	# no jump src1 >= src2
	IMM TMP0
	TMP0 PC
	TMP0
	TMP2 TMP2 END
blt-no-jump:
    	# src1 < src2 jump
	NEXT PC END

@BGE
	# check src1 <= src2
	SRC1 SRC2 bge-jump
bge-no-jump:
	# no jump src1 >= src2
	NEXT PC END
bge-jump:
	# src1 < src2 jump
	IMM TMP0
	TMP0 PC
	TMP0
	TMP2 TMP2 END

@BLTU
    #  jump if src1 < src2 unsigned
bltu:
	WORD TMP5
	TMP0 TMP0 bltu-check
bltu-loop:
	TMP4
	SRC1 TMP4
	TMP4 SRC1
	TMP4
	SRC2 TMP4
	TMP4 SRC2
	TMP4
bltu-check:
	TMP3
	SRC2 TMP3 bltu-srctwo-pos
	TMP3
	SRC1 TMP3 bltu-jump
	TMP3 TMP3 bltu-next-bit
bltu-srctwo-pos:
	TMP3
	SRC1 TMP3 bltu-next-bit
	TMP0 TMP0 bltu-next-instruction
bltu-next-bit:
	TMP0
	TMP5 TMP0 bltu-next-instruction
	INC TMP5
	TMP0 TMP0 bltu-loop
bltu-next-instruction:
	TMP0
	TMP3
	TMP4
	TMP5
	NEXT PC END
bltu-jump:
	TMP0
	TMP3
	TMP4
	TMP5
	IMM TMP0
	TMP0 PC
	TMP0 TMP0 END

@BGEU
bgeu:
	WORD TMP5
	TMP0 TMP0 bgeu-check
bgeu-loop:
	TMP4
	SRC1 TMP4
	TMP4 SRC1
	TMP4
	SRC2 TMP4
	TMP4 SRC2
	TMP4
bgeu-check:
	TMP3
	SRC2 TMP3 bgeu-srctwo-pos
	TMP3
	SRC1 TMP3 bgeu-jump
	TMP3 TMP3 bgeu-next-bit
bgeu-srctwo-pos:
	TMP3
	SRC1 TMP3 bgeu-next-bit
	TMP0 TMP0 bgeu-next-instruction
bgeu-next-bit:
	TMP0
	TMP5 TMP0 bgeu-jump
	INC TMP5
	TMP0 TMP0 bgeu-loop
bgeu-next-instruction:
	TMP0
	TMP3
	TMP4
	TMP5
	NEXT PC END
bgeu-jump:
	TMP0
	TMP3
	TMP4
	TMP5
	IMM TMP0
	TMP0 PC
	TMP0 TMP0 END

# ============================================================
#                         BIT LOGIC
# ============================================================
@XORI
@XOR
	RXOR
	SRC2 TMP0
	TMP0 RACC
	TMP0
	IMM TMP0
	TMP0 RXOR
	TMP0
	SRC2
	RACC
	RXOR TMP0
	TMP0 RSLT
	TMP0
	NEXT PC END

@ANDI
@AND
	RAND
	SRC2 TMP0
	TMP0 RACC
	TMP0
	IMM TMP0
	TMP0 RAND
	TMP0
	SRC2
	RACC
	RAND TMP0
	TMP0 RSLT
	TMP0
	NEXT PC END

@ORI
@OR
    mapping[OR] = 606;
	ROR
	SRC2 TMP0
	TMP0 RACC
	TMP0
	IMM TMP0
	TMP0 ROR
	TMP0
	SRC2
	RACC
	ROR TMP0
	TMP0 RSLT
	TMP0
	NEXT PC END    


# ============================================================
#                         SHIFTING
# ============================================================
@SLLI
@SLL
sll:
	SRC1 TMP0 sll-shift	# -SRC2 -> TMP0
sll-loop:    
	SRC2 TMP1		# negate shift value
sll-shift:	
	TMP1 SRC2     	        # SRC2 + SRC2
	TMP1			# reset TMP1
	INC TMP0 sll-loop	# loop
sll-clean:    
	TMP0			# clean
	TMP1     		# clean
	NEXT PC END		# next RISC-V

@SRAI
@SRA
	SRC1 TMP0 sra-is-pos
sra-is-neg:			# set RSLT to -1 to sign extend
	TMP0
	INC TMP0
	TMP0 RSLT
	TMP0 TMP0 srl
sra-is-pos:
	TMP0

@SRLI
@SRL
srl:
	WORD TMP5
	IMM TMP0
	TMP0 TMP5
srl-msb-check:
	TMP0
	SRC1 TMP0 srl-msb-unset
srl-msb-set:	
	TMP0
	RSLT TMP0	# RSLT <<1
	TMP0 RSLT	# RSLT <<1	
	INC RSLT	# set bit of RSLT
	TMP0 TMP0 srl-check-loop
srl-msb-unset:
	TMP0
	RSLT TMP0	# RSLT <<1
	TMP0 RSLT	# RSLT <<1	
srl-check-loop:
	TMP0
	TMP5 TMP0 srl-end	
srl-move-op:
	TMP0
	SRC1 TMP0
	TMP0 SRC1
	INC TMP5	
	TMP0 TMP0 srl-msb-check	
srl-end:
	TMP0
	TMP5
	NEXT PC END



# ============================================================
#                         SET BIT
# ============================================================
@SLTI
@SLT
slt-check-one:
	SRC1 SRC2 slt-no-set
slt-set:    
	SRC2
	INC SRC2
	NEXT PC END
slt-no-set:
	SRC2
	NEXT PC END

@SLTIU
@SLTU
# jump if src1 < src2 unsigned
sltu:
	WORD TMP5 # TMP5 is loop counter
	TMP0 TMP0 sltu-check
sltu-loop:
	TMP4
	SRC1 TMP4  # shift src1 <<
	TMP4 SRC1
	TMP4
	SRC2 TMP4  # shift src2 <<
	TMP4 SRC2
	TMP4
sltu-check:
	TMP3
	SRC2 TMP3 sltu-srctwo-pos
	# here src2 is negative src2
	TMP3
	SRC1 TMP3 sltu-jump # if jump then src1 && !src2
	# here !src1 && !src2 therefore go to next bit
	TMP3 TMP3 sltu-next-bit
sltu-srctwo-pos:
	# here src1 is negative
	TMP3
	SRC1 TMP3 sltu-next-bit
	# here !src1 && src2
	TMP0 TMP0 sltu-next-instruction
sltu-next-bit:
	TMP0
	TMP5 TMP0 sltu-next-instruction # if TMP5 == 0
	INC TMP5
	TMP0 TMP0 sltu-loop
sltu-next-instruction:
	# here we have an error dont jump
	RSLT RSLT sltu-exit
sltu-jump:
	RSLT
	INC RSLT
sltu-exit:
	TMP0
	TMP3
	TMP4
	TMP5
	NEXT PC END

