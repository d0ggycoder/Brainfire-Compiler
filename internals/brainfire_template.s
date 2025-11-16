.set BUF_SIZE, 128
.section .data
    oob_err: .asciz "Error, attempted to access out of bounds data\n"
    oob_len = .-oob_err

.section .bss
    buf: .space BUF_SIZE

.section .text
.global _start

_start:
    # Use %rax as the buffer pointer 
    lea buf(%rip), %rax
    call main

_end:
    mov $60, %rax 
    xor %rdi, %rdi
    syscall

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

# Instruction for printing byte (.)
print_byte:
    # Push registers
    push %rax
    push %rdi
    push %rsi
    push %rdx

    # Print syscall
    mov %rax, %rsi      # message
    mov $1, %rax        # syscall: write
    mov $1, %rdi        # stdout
    mov $1, %rdx        # length
    syscall

    # Pop registers
    pop %rdx
    pop %rsi
    pop %rdi
    pop %rax
    ret

# Instruction for right pointer shift (>)
shift_r:
    inc %rax
    cmp $BUF_SIZE, %rax 
    jnz shift_r_safe
    mov $1, %rdi
    jmp p_error
    shift_r_safe:
    ret

# Instruction for left pointer shift (<)
shift_l:
    cmp $0, %rax
    jnz shift_l_safe
    mov $1, %rdi
    jmp p_error
    shift_l_safe:
    dec %rax
    ret

main:
