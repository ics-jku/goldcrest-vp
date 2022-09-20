.include "../util/print.S"
.global _start

# program entry-point
_start:

beq_test1:
	print_str "beq    (1==1): "
	li a0, 1
	li a1, 1
	beq a0, a1, beq_test2
	j fail

beq_test2:
	print_str "PASS\nbeq  (-2==-2): "
	li a0, -2
	li a1, -2
	beq a0, a1, beq_test3
	j fail

beq_test3:
	print_str "PASS\nbeq    (2==1): "
	li a0, 2
	li a1, 1
	beq a0, a1, fail

beq_test4:
	print_str "PASS\nbeq    (1==2): "
	li a0, 1
	li a1, 2
	beq a0, a1, fail
	
beqz_test1:
	print_str "PASS\nbeqz      (0): "
	li a0, 0
	beqz a0, beqz_test2
	j fail

beqz_test2:
	print_str "PASS\nbeqz      (1): "
	li a1, 1
	beqz a1, fail
	print_str "PASS\n"
	j exit
	
fail:
	print_str "FAIL <===\n"
exit:
	sw x0, 1020(x0)
