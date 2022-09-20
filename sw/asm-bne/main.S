.include "../util/print.S"
.global _start

fail:
	print_str "FAIL <===\n"
exit:
	sw x0, 1020(x0)
	
_start:
	nop
bne_test1:
	print_str "bne    (1!=1): "
	li a0, 1
	li a1, 1
	bne a0, a1, fail

bne_test2:
	print_str "PASS\nbne  (-2!=-2): "
	li a0, -2
	li a1, -2
	bne a0, a1, fail

bne_test3:
	print_str "PASS\nbne    (2!=1): "
	li a0, 2
	li a1, 1
	bne a0, a1, bne_test4
	j fail

bne_test4:
	print_str "PASS\nbne    (1!=2): "
	li a0, 1
	li a1, 2
	bne a0, a1, bne_test5
	j fail

bne_test5:
	print_str "PASS\nbne   (-2!=1): "
	li a0, -2
	li a1, 1
	bne a0, a1, bne_test6
	j fail

bne_test6:
	print_str "PASS\nbne   (2!=-1): "
	li a0, 2
	li a1, -1
	bne a0, a1, bne_test7
	j fail

# jump back
1:
	j bne_test8
bne_test7:
	print_str "PASS\nbne    (2!=1): "
	li a0, 2
	li a1, 1
	bne a0, a1, 1b

1:
	j bne_test9
bne_test8:
	print_str "PASS\nbne   (-2!=1): "
	li a0, -2
	li a1, 1
	bne a0, a1, 1b

bne_test9:
	# rs2_val == -2, rs1_val == -67108865
	# opcode: bne, op1:x10; op2:x11; op1val:-0x4000001; op2val:-0x2; immval:0x400; align:0
	print_str "PASS\nbne  (-2!=-67108865): "
	li x10, -2
	li x11, -67108865
	bne x10, x11, bne_test10
	j fail
	.rept 98
	nop
	.endr

bne_test10:
	# rs2_val == -2147483648, 
	# opcode: bne, op1:x10; op2:x11; op1val:0x0; op2val:-0x80000000; immval:0x4; align:0
	print_str "PASS\nbne  (0!=-2147483648): "
	li x10, 0
	li x11, -2147483648
	
	#print_str "\n"
	#sub x12, x10, x11
	#print_num_reg x11
	#print_str "\n"
	#print_num_reg x12
	#print_str "\n"
	
	bne x10, x11, pass
	j fail

pass:
	print_str "PASS\n"
	j exit
