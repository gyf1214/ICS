.globl rsp
rsp:
    .org rsp + 0x18, 0x23

rbp:
    .quad rsp

tail:
    .byte 0xa
