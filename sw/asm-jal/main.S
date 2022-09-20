.include "../util/print.S"
.global _start

_start:

jal_test1:
	print_str "jal1         : "
	jal t2, jump_target
link_target:
	addi t0, t0, 1		# dummy should not execute
	addi t0, t0, 1		# dummy should not execute
jump_target:
	la t0, link_target
	beq t2, t0, pass
	j fail

fail:
	print_str "FAIL <===\n"
	sw x0, 1020(x0)
pass:
	print_str "PASS\n"
	sw x0, 1020(x0)
