#.include "../util/print.S"
.global _start

_start:
	li x5, 0x33333334
	li x18, -0x80000000
	slt x7, x5, x18
	sw x18, 1016(zero)

exit:	sw x0, 1020(x0)
