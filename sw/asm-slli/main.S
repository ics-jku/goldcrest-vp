.include "../util/print.S"
.global _start

_start:

slli1:
	print_str "slli   (1<<4): "
	li t0, 1
	slli t1, t0, 4
	li t2, 16
	bne t1, t2, fail
	print_str "PASS\n"

slli2:
	print_str "slli   (3<<9): "
	li t0, 3
	slli t1, t0, 9
	li t2, 1536
	bne t1, t2, fail
	print_str "PASS\n"

slli3:
	print_str "slli  (-1<<1): "
	li t0, -1
	slli t1, t0, 1
	li t2, -2
	bne t1, t2, fail
	print_str "PASS\n"

slli4:
	print_str "slli (-1<<31): "
	li t0, -1
	slli t1, t0, 31
	li t2, -2147483648
	bne t1, t2, fail
	print_str "PASS\n"

slli5:
	print_str "slli  (8<<31): "
	li t0, 8
	slli t1, t0, 31
	li t2, 0
	bne t1, t2, fail
	print_str "PASS\n"
	j exit	
	
fail:
	print_str "FAIL <=== "
	
exit:	sw x0, 1020(x0)
