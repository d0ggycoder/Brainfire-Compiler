.data
BUF_SIZE = 128
format_byte: .asciz "Testing"
.LC0:
.ascii "Hello, world\n\0"


.bss
.align 16
buf:
    .zero 128

.text
    .globl main
    .globl BUF_SIZE
    .extern printf
    #.extern shift_l
    #.extern shift_r
    #.extern p_byte
    #.extern p_char

main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp

    leaq buf(%rip),%rax
    movb $69, (%rax)

	movl	$0, %eax
	addq	$32, %rsp
	popq	%rbp
	ret
