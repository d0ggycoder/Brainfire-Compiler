.section .rdata 
    oob_err: .asciz "Error, pointer out of bounds"
    oob_len = .-oob_err

    format_char: .asciz "%c"
    format_byte: .asciz "%d"

.text
.extern BUF_SIZE
.globl shift_r
.globl shift_l
.globl p_byte
.globl p_char

# Handle Errors 
p_error:    
    push %rdi

    cmp $1, %rdi
    jnz p_error_end # Jump to the next check (or end)
        # Error code 1: out of bounds
        mov $1, %rax
        mov $1, %rdi
        lea oob_err(%rip), %rsi
        mov $oob_len, %rdx
        syscall
        jmp p_error_end

    p_error_end:
        # End with error code based on rdi
        pop %rdi 
        mov $60, %rax 
        syscall

shift_r:
    inc %rax
    cmp $BUF_SIZE, %rax 
    jnz shift_r_safe
    mov $1, %rdi
    jmp p_error
    shift_r_safe:
    ret

shift_l:
    cmp $0, %rax
    jnz shift_l_safe
    mov $1, %rdi
    jmp p_error
    shift_l_safe:
    dec %rax
    ret

p_byte:
    pushq %rax
    subq $32, %rsp
    movzx (%rax), %rdx        # printf arg2 = result
    leaq format_byte(%rip), %rcx # printf arg1 = format string
    call printf
    addq $32, %rsp
    popq %rax
    ret

p_char:
    pushq %rax
    subq $32, %rsp
    movzx (%rax), %rdx        # printf arg2 = result
    leaq format_char(%rip), %rcx # printf arg1 = format string
    call printf
    addq $32, %rsp
    popq %rax
    ret
