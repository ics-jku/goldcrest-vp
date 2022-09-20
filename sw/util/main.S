.include "print.S"
.global _start

_start:
	li sp, 2000
	li gp, 3000
main:	
	print_str "123\n"
	call FN_exit

