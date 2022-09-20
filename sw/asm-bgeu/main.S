.include "../util/print.S"
.global _start

# program entry-point
_start:

bgeu_test1:
	print_str "bgeu   (1>=1): "
	li a0, 1
	li a1, 1
	bgeu a0, a1, bgeu_test2
	j fail

bgeu_test2:
	print_str "PASS\nbgeu   (2>=2): "
	li a0, 2
	li a1, 2
	bgeu a0, a1, bgeu_test3
	j fail

bgeu_test3:
	print_str "PASS\nbgeu   (2>=1): "
	li a0, 2
	li a1, 1
	bgeu a0, a1, bgeu_test4
	j fail

bgeu_test4:
	print_str "PASS\nbgeu  (-2>=1): "
	li a0, -2
	li a1, 1
	bgeu a0, a1, bgeu_test5
	j fail

bgeu_test5:
	print_str "PASS\nbgeu (-2>=-1): "
	li a0, -2
	li a1, -1
	bgeu a0, a1, fail
	
bgeu_test6:
	print_str "PASS\nbgeu   (1>=2): "
	li a0, 1
	li a1, 2
	bgeu a0, a1, fail
	print_str "PASS\n"
	jal exit
	
fail:
	print_str "FAIL <===\n"
exit:
	sw x0, 1020(x0)
