#.include "../util/print.S"
.global _start

_start:
	li t0, 8
	li t1, 1
	xor t2, t0, t1
	sw t2, 1016(zero)

exit:	sw x0, 1020(x0)
