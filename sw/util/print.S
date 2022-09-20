.macro print_str str
.section .data
txt\@:
	.string "\str"
	.word 0
.section .text.init
	la a0, txt\@
	jal FN_print_str
.endm

.macro print_num num
	li a0, \num
	jal FN_print_num
.endm

.macro print_num_reg reg
	mv a0, \reg
	jal FN_print_num
.endm
	
FN_print_str:
	lw t0, 0(a0)
	beqz t0, write_exit
	addi a0, a0, 1
	sw t0, 1024(x0)
	j FN_print_str
write_exit:
	ret

FN_print_num:
	sw a0, 1016(x0)
	ret

FN_exit:
	sw x0, 1020(x0)
