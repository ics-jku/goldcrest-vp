! Compliant
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
	TMP0
	IMM  TMP0		# negate IMM   | TMP0 - IMM = 0 - mem[IMM]
	TMP0 SRC2     	  	# SRC2 - -(TMP0)
addi-exit:	
	NEXT PC END		# increment RISC-V pc

@SUB
	TMP0
	SRC2 IMM		# SRC2 - -(TMP0)
	SRC2 SRC2
	IMM TMP0
	TMP0 SRC2
	NEXT PC END		# increment RISC-V pc

@AUIPC
	TMP0
	SRC2
	PC TMP0			# copy PC to SRC2
	TMP0 SRC2
	TMP0			# PC + IMM
	IMM TMP0
	TMP0 SRC2
	NEXT PC END

@LUI
	TMP0
	IMM TMP0
	TMP0 SRC2
	NEXT PC END

# ============================================================
#                         JUMPS
# ============================================================
@JAL
	TMP0
	TMP1
	SRC2			# reset SRC2
	IMM  TMP0	      	# negate IMM
	PC   TMP1     	       	# -pc => TMP1
	TMP1 SRC2     	       	# pc => SRC2
	NEXT SRC2              	# add link address +4
	TMP0 PC END    	       	# PC - -TMP0

@JALR
	TMP0     		# reset TMP0
	TMP1     		# reset TMP1
	TMP2     		# reset TMP2
	TMP3 			# reset TMP3
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
	TMP3 SRC2 END  		# (PC + 4) => SRC2

# ============================================================
#                         BRANCHES
# ============================================================
@BEQ
	TMP0
	TMP1
	SRC2 TMP0		# copy SRC2 to TMP1
	TMP0 TMP1
beq-check-one:
	SRC1 SRC2 beq-check-two
	TMP0 TMP0 beq-no-jump-two # NO
beq-check-two:	
	TMP1 SRC1 beq-jump	# test if SRC1 <= SRC2
beq-no-jump-two:
	NEXT PC END    		# goto next instruction
beq-jump:
	TMP0
	IMM TMP0       		# YES
	TMP0 PC END    		# pc + B_imm

@BNE
	TMP0
	TMP1
	SRC2 TMP0		# copy SRC2 to TMP1
	TMP0 TMP1
	SRC1 SRC2 bne-check-two	# test if SRC2 <= SRC1
	TMP0 TMP0 bne-jump
bne-check-two:
	TMP1 SRC1 bne-no-jump   # test if SRC1 <= SRC2
bne-jump:
	TMP2
	IMM TMP2
	TMP2 PC END
bne-no-jump:	
	NEXT PC END

@BLT
	# check src1 <= src2
	SRC1 SRC2 blt-no-jump
blt-jump:
	TMP0
	TMP1
	# no jump src1 >= src2
	IMM TMP0
	TMP0 PC END
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
	TMP0
	IMM TMP0
	TMP0 PC END

@BLTU
    #  jump if src1 < src2 unsigned
    	TMP0
	TMP3
	TMP4
	TMP5
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
	#TMP3
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
	NEXT PC END
bltu-jump:
	IMM TMP0
	TMP0 PC END

@BGEU
	TMP0
	TMP3
	TMP4
	TMP5
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
	NEXT PC END
bgeu-jump:
	IMM TMP0
	TMP0 PC END

# ============================================================
#                         BIT LOGIC
# ============================================================
@XORI
@XOR
	TMP0			 
	TMP1
	TMP2
	TMP3
	TMP4
	TMP5
	SRC1
	SRC2 TMP0 # move SRC2 to SRC1
	TMP0 SRC1
	RSLT
	TMP0
	WORD TMP5
	ONE TMP4
xor-loop:
	RSLT TMP3  # shift result <<
	TMP3 RSLT
	TMP3
xor-get-msb-src-one:
        TMP0
	SRC1 TMP0 xor-get-msb-src-two
	INC TMP1
xor-get-msb-src-two:
        TMP0
	TMP2
	IMM TMP0 xor-get-msb-clean
	INC TMP2
xor-get-msb-clean:
	TMP0
xor-add:
	TMP1 TMP3
	TMP3 TMP2  # TMP1 + TMP2
	TMP3
xor-check-one:
	ONE TMP2 xor-check-two # test TMP2 <= ONE
	TMP3 TMP3 xor-shift # TMP2 == 2
xor-check-two:    
	INC TMP2     # TMP2+1 to undo previous check
	TMP3          # get copy of one
	TMP4 TMP3    # -ONE in TMP4
	TMP2 TMP3 xor-set-bit # test ONE <= TMP2
	TMP3 TMP3 xor-shift # TMP2 == 0
xor-set-bit:
	INC RSLT
xor-shift:
	SRC1 TMP3
	TMP3 SRC1
	TMP3
	IMM TMP3
	TMP3 IMM
	TMP3
	INC TMP5 xor-loop
xor-clean:
	NEXT PC END

@ORI
@OR
	TMP0			 
	TMP5
	SRC1
	SRC2 TMP0 # move SRC2 to SRC1
	TMP0 SRC1
	RSLT
	WORD TMP5
or-loop:
	TMP0
	RSLT TMP0  # shift result <<
	TMP0 RSLT
or-get-msb-src-one:
        TMP0
	SRC1 TMP0 or-get-msb-src-two
	TMP0 TMP0 or-set-bit
or-get-msb-src-two:
        TMP0
	IMM TMP0 or-shift
or-set-bit:
	INC RSLT
or-shift:
	TMP0
	SRC1 TMP0
	TMP0 SRC1
	TMP0
	IMM TMP0
	TMP0 IMM
	TMP0
	INC TMP5 or-loop
or-clean:
	NEXT PC END

@ANDI
@AND
	TMP0			 
	TMP3
	TMP5
	SRC1
	SRC2 TMP0 # move SRC2 to SRC1
	TMP0 SRC1
	RSLT
	WORD TMP5
and-loop:
	TMP3
	RSLT TMP3  # shift result <<
	TMP3 RSLT
and-get-msb-src-one:
        TMP0
	SRC1 TMP0 and-shift # if MSB is 0 shift
and-get-msb-src-two:
        TMP0
	IMM TMP0 and-shift # if MSB is 0 shift
and-set-bit: # here, TMP2 must be 2
	INC RSLT
and-shift:
	TMP3
	SRC1 TMP3
	TMP3 SRC1
	TMP3
	IMM TMP3
	TMP3 IMM
	INC TMP5 and-loop
and-clean:
	NEXT PC END

# ============================================================
#                         SHIFTING
# ============================================================
@SLLI
@SLL
	TMP0
	TMP1
	SRC1 TMP0 sll-shift	# -SRC2 -> TMP0
sll-loop:    
	SRC2 TMP1		# negate shift value
sll-shift:	
	TMP1 SRC2     	        # SRC2 + SRC2
	TMP1			# reset TMP1
	INC TMP0 sll-loop	# loop
sll-clean:    
	NEXT PC END		# next RISC-V

@SRAI
@SRA
	TMP0
	SRC1 TMP0 sra-is-pos
sra-is-neg:			# set RSLT to -1 to sign extend
	TMP0
	INC TMP0
	TMP0 RSLT
sra-is-pos:


@SRLI
@SRL
srl:
	TMP0
	TMP5
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
        TMP0
	TMP3
	TMP4
	TMP5
	WORD TMP5 # TMP5 is loop counter
	TMP0 TMP0 sltu-check
sltu-loop:
	TMP4
	SRC1 TMP4  # shift src1 << 1
	TMP4 SRC1
	TMP4
	SRC2 TMP4  # shift src2 << 1
	TMP4 SRC2
	TMP4
sltu-check:
	TMP3
	SRC2 TMP3 sltu-srctwo-pos
	# here src2 is negative, src2
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
	# here we have an error, dont jump
	RSLT RSLT sltu-exit
sltu-jump:
	RSLT
	INC RSLT
sltu-exit:
	NEXT PC END
