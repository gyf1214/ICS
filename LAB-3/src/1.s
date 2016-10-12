.globl rsp
rsp:
    .org rsp + 0x18, 0x23

rbp:
    .quad 0x401863

tail:
    .byte 0xa
