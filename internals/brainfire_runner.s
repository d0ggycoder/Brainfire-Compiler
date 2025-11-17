.data
BUF_SIZE = 128

.bss
.align 16
buf:
    .zero 128

.text
    .globl main
    .globl BUF_SIZE
    .extern shift_l
    .extern shift_r
    .extern p_byte
    .extern p_char

main:
    subq $32, %rsp         # shadow space

    lea buf(%rip),%eax
    movb $69, (%eax)
    call p_byte

    xor %eax, %eax         # return
    addq $32, %rsp
    ret
