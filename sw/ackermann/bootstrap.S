.globl _start
.globl main

_start:
	li sp, 50000
	jal main
	sw x0, 1020(x0)
