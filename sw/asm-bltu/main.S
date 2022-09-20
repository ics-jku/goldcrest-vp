.include "../util/print.S"
.global _start

# program entry-point
_start:

bltu_test1:
	print_str "bltu    (1<1): "
	li a0, 1
	li a1, 1
	bltu a0, a1, fail

bltu_test2:
	print_str "PASS\nbltu    (2<2): "
	li a0, 2
	li a1, 2
	bltu a0, a1, fail

bltu_test3:
	print_str "PASS\nbltu    (2<1): "
	li a0, 2
	li a1, 1
	bltu a0, a1, fail

bltu_test4:
	print_str "PASS\nbltu   (-2<1): "
	li a0, -2
	li a1, 1
	bltu a0, a1, fail

bltu_test5:
	print_str "PASS\nbltu  (-2<-1): "
	li a0, -2
	li a1, -1
	bltu a0, a1, bltu_test6
	j fail
	
bltu_test6:
	print_str "PASS\nbltu    (1<2): "
	li a0, 1
	li a1, 2
	bltu a0, a1, 1f
	j fail
1:	
	print_str "PASS\n"
	jal exit
	
fail:
	print_str "FAIL <===\n"
exit:
	sw x0, 1020(x0)
