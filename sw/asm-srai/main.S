.include "../util/print.S"
.global _start

_start:

srai1:
	print_str "srai   (2>>1): "
	li t0, 2
	srai t1, t0, 1
	li t2, 1
	bne t1, t2, fail
	print_str "PASS\n"

srli2:
	print_str "srai   (4>>1): "
	li t0, 4
	srai t1, t0, 1
	li t2, 2
	bne t1, t2, fail
	print_str "PASS\n"

srli3:
	print_str "srai (-2>>-1): "
	li t0, -2
	srai t1, t0, 1
	li t2, -1
	bne t1, t2, fail
	print_str "PASS\n"
	j exit
	
fail:
	print_num_reg t1
	print_str "FAIL <=== "
	
exit:	sw x0, 1020(x0)
