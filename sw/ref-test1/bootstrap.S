.globl _start
.globl main

_start:
	li sp, 200000
	jal main
	sw x0, 1020(x0)
