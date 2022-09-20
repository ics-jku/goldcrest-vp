.include "../util/print.S"
.global _start

_start:

srli1:
	print_str "srli   (2>>1): "
	li t0, 2
	srli t1, t0, 1
	print_num_reg t1	
	li t2, (2>>1)
	bne t1, t2, 1f
1:	

	print_str "PASS\n"

srli2:
	print_str "srli   (8>>1): "
	li t0, 8
	srli t1, t0, 1
	li t2, (8>>1)
	bne t1, t2, fail
	print_str "PASS\n"

srli3:
	print_str "srli   (4>>2): "
	li t0, 4
	srli t1, t0, 2
	li t2, (4>>2)
	bne t1, t2, fail
	print_str "PASS\n"

srli4:
	print_str "srli   (15>>3): "
	li t0, 15
	srli t1, t0, 3
	li t2, (15>>3)
	bne t1, t2, fail
	print_str "PASS\n"

srli5:
	print_str "srli   (15>>0): "
	li t0, 15
	srli t1, t0, 0
	li t2, (15>>0)
	bne t1, t2, fail
	print_str "PASS\n"

srli6:
	print_str "srli   (-1>>1): "
	li t0, -1
	srli t1, t0, 1
	li t2, 0x7FFFFFFF
	sw t1, 1016(x0)
	bne t1, t2, fail
	print_str "PASS\n"
	
	j exit	

	
	
fail:
	print_str "FAIL <=== "
	
exit:	sw x0, 1020(x0)
