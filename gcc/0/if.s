	.file	"if.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$42, -4(%rbp)
	cmpl	$114513, -4(%rbp)
	jg	.L2
	movl	$1919810, -4(%rbp)
	movl	-4(%rbp), %eax
	jmp	.L3
.L2:
	cmpl	$810, -4(%rbp)
	jle	.L4
	movl	$810, -4(%rbp)
	movl	-4(%rbp), %eax
	jmp	.L3
.L4:
	movl	$-1, -4(%rbp)
	movl	-4(%rbp), %eax
.L3:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.2.0-17ubuntu2) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
