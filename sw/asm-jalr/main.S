.include "../util/print.S"
.global _start

_start:

jalr_test1:
	print_str "jalr1  (addr): "
	la t0, jump_target1
	jalr x0, t0, 0
	j fail
link_target1:
	addi t0, t0, 1		# dummy should not execute
	addi t0, t0, 1		# dummy should not execute
jump_target1:
	print_str "PASS\n"


jalr_test2:
	print_str "jalr2   (off): "
	la t0, link_target2
	jalr x0, t0, 12
link_target2:
	j fail
	addi t0, t0, 1		# dummy should not execute
	addi t0, t0, 1		# dummy should not execute
jump_target2:
	print_str "PASS\n"


jalr_test3:
	print_str "jalr3  (link): "
	la t0, jump_target3
	jalr t0
link_target3:
	j fail
	addi t0, t0, 1		# dummy should not execute
	addi t0, t0, 1		# dummy should not execute
jump_target3:
	la t1, link_target3
	beq ra, t1, pass
	j fail

fail:
	print_str "FAIL <===\n"
	sw x0, 1020(x0)
pass:
	print_str "PASS\n"	
	sw x0, 1020(x0)
