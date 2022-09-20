.include "../util/print.S"
.include "data.s"	

.global _start

.text
_start:
	li sp, 1024
	print_str "Start: \n"
	li t2, 1
	li t3, 0
	la t4, data
	li s0, 0
	li s1, 0

loop:
	add t5, t4, t3
	lb t6, 0(t5)
	beq t6, zero, exit
	addi t6, t6, -40
	beq t6, zero, inc
dec:	addi s0, s0, -1
	blt s0, zero, basement
	j next
inc:	addi s0, s0, 1
next:	addi t3, t3, 1
	j loop

basement:
	bnez s1, next
	mv s1, t3
	addi s1, s1, 1
	j next
	
exit:	print_num_reg s0
	print_str "\n"
	print_num_reg s1
	print_str "\n"	
	sw x0, 1020(x0)
