.section .text
.globl _start
	
_start:
	la a0, txt
	call FN_WriteStr
	la a0, piep
	call FN_WriteStr
	sw x0, 1020(x0)

FN_WriteStr:
	li t1, 1		# branch compare
print:	
	lw a1, 0(a0)		# read from string
	addi a0, a0, 1		# inc string offset
	blt a1, t1, print_exit	# check if end of string seen
	sw a1, 1024(x0)		# print read byte
	j print			# loop to get next byte
print_exit:
	ret

.section .rodata
txt:  .string "Hello, world!\n\0\0\0"
piep: .string "Piep piep\n\0\0\0\0"
