.global _start

.section .data.
num:
    .word 22			

.section .text

_start:
	la t0, num
	lw a0, 0(t0)

exit:	sw x0, 1020(x0)
