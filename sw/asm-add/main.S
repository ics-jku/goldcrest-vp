.include "../util/print.S"
.global _start

_start:

add1:
	print_str "add1  (14+23): "
	li t0, 14
	li t1, 23
	li t2, 37
	add t3, t0, t1
	bne t3, t2, fail
	print_str "PASS\n"
	
add2:
	print_str "add2 (-14+23): "
	li t0, -14
	li t1, 23
	li t2, (-14+23)
	add t0, t0, t1
	bne t0, t2, fail
	print_str "PASS\n"
	
add3:
	print_str "add3  (14-23): "
	li t0, 14
	li t1, -23
	li t2, (14-23)
	add t0, t0, t1
	bne t0, t2, fail
	print_str "PASS\n"
	
	j exit
fail:
	print_str "FAIL <=== "
	print_num_reg t3
	print_str " != "
	print_num_reg t2
	
exit:	sw x0, 1020(x0)
