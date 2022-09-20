.include "../util/print.S"
.global _start

_start:

blt_test1:
	print_str "blt     (1<1): "
	li a0, 1
	li a1, 1
	blt a0, a1, fail

beq_test2:
	print_str "PASS\nblt   (-2<-2): "
	li a0, -2
	li a1, -2
	blt a0, a1, fail

beq_test3:
	print_str "PASS\nblt     (2<1): "
	li a0, 2
	li a1, 1
	blt a0, a1, fail

beq_test4:
	print_str "PASS\nblt     (1<2): "
	li a0, 1
	li a1, 2
	blt a0, a1, 1f
	j fail
1:	print_str "PASS\n"

beq_test5:
	print_str "blt    (-2<1): "
	li a0, -2
	li a1, 1
	blt a0, a1, 1f
	j fail
1:	print_str "PASS\n"

beq_test6:
	print_str "blt    (2<-1): "
	li a0, 2
	li a1, -1
	blt a0, a1, fail
	print_str "PASS\n"

beq_test7:
	print_str "blt   (-2<-1): "
	li a0, -2
	li a1, -1
	blt a0, a1, 1f
	j fail
1:	print_str "PASS\n"

beq_test8:
	print_str "blt   (-2<-5): "
	li a0, -2
	li a1, -5
	blt a0, a1, fail
1:	print_str "PASS\n"

beq_test9:
	print_str "blt          : "
	li a0, 0x55555555
	li a1, 0x5
	blt a0, a1, fail
	print_str "PASS\n"
	j exit

	
fail:
	print_str "FAIL <===\n"
exit:
	sw x0, 1020(x0)
