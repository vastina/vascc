	.file	"print.c"
.LC0:
	.string	"Hello World"
	.globl	main
	.type	main, @function
main:
.LFB23:
	endbr64
	subq	$8, %rsp
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	ret
.LFE23:
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"

# remove something not important for codegen