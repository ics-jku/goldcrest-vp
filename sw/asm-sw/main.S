.include "../util/print.S"
.global _start

# program entry-point
_start:

# check positive offset calculation
sw_test1:
	print_str "sw +         : "
	li t0, 23
	li a0, 2000
	sw t0, 20(a0)
	
	li a1, 2020
	lw t1, 0(a1)
	li t2, 23
	beq t1, t2, sw_test2
	j fail
	
# check negative offset calculation
sw_test2:
	print_str "PASS\nsw -         : "
	li t0, 23
	li a0, 2000
	sw t0, -20(a0)
	
	li a1, 1980
	lw t1, 0(a1)
	li t2, 23
	beq t1, t2, pass
	j fail
	
	
fail:
	print_str "FAIL <===\n"
	sw x0, 1020(x0)	
pass:
	print_str "PASS\n"
	sw x0, 1020(x0)
