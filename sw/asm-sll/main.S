.include "../util/print.S"
.global _start

_start:

sll1:
	print_str "sll    (1<<4): "
	li t0, 1
	li t1, 4
	sll t1, t0, t1
	li t2, 16
	bne t1, t2, fail
	print_str "PASS\n"

sll2:
	print_str "sll    (3<<9): "
	li t0, 3
	li t1, 9
	sll t1, t0, t1
	li t2, 1536
	bne t1, t2, fail
	print_str "PASS\n"

sll3:
	print_str "sll   (-1<<1): "
	li t0, -1
	li t1, 1
	sll t1, t0, t1
	li t2, -2
	bne t1, t2, fail
	print_str "PASS\n"

sll4:
	print_str "sll  (-1<<31): "
	li t0, -1
	li t1, 31
	sll t1, t0, t1
	li t2, -2147483648
	bne t1, t2, fail
	print_str "PASS\n"

sll5:
	print_str "sll   (8<<31): "
	li t0, 8
	li t1, 31
	sll t1, t0, t1
	li t2, 0
	bne t1, t2, fail
	print_str "PASS\n"

sll6:
	print_str "sll          : "
	li t0, 	-0x8001
	li t1, -0x8001
	sll t1, t0, t1
	li t2, 0x80000000
	bne t1, t2, fail
	print_str "PASS\n"
	j exit

fail:
	print_str "FAIL <=== "
	
exit:	sw x0, 1020(x0)
