# LAB-3 Notes #

Fuck tao@ubuntu64!

## how to debug offline ##

> The same as Lab-2

## how to generate your exploit string in assembly ##

First, just use assembly language to build your string.

You should know that when `Gets()` is called, the string is stored in stack from `$rsp`, so I label the first line as `rsp`.

After BUFFER_SIZE, it's the end of the stack frame of `getbuf()`, which stores of return address. Padding is simple using AT&T op `.org`.

Now it comes to the overflow part, which I label as `rbp`.

After everything, a `\n`(`0xa`) should be given.

The `rsp` label should be set as global as the entry point.

### assembly ###

Just asm to obj.

    $ as 1.s -o 1.o

### linking ###

There are relative addresses in your inject for ctarget. (rtarget cannot use the address of stack)

In addition, `ld` can output bare binary:

    $ ld -e rsp -Ttext <your rsp address in getbuf()> --oformat binary 1.o -o 1

See details in `makefile`.

## phase 1 ##

Simply set the return address to `touch1()`.

## phase 2 ##

Set the return address to somewhere in the stack, and put your code there.

    movl $<your cookie>, %edi

Do not use `jmp` or `call`, just do as below:

    pushq $0x40188f
    retq

To make a jump or call. (of course you don't need an actual call)

You can use label to automatically calculate the address in stack in linking:

    rsp:
    /* ... */       /* your code */
    rbp:            /* the return address */
        .quad rsp   /* it will be calculated in linking */

    tail:
        .byte 0xa

> Note: cookie won't change, so there is no need to access the memory.

## phase 3 ##

First put the cookie string in somewhere __AFTER__ the return address, since when the inject code & touch functions are executing, `$rsp` is pointed to the bottom of `getbuf()`, and new things will cover the inject data.

Use `.asciz` to delare a zero-end string:

    ...
    rbp:
        .quad rsp

    data:
        .asciz "<your cookie>"

    tail:
        .byte 0xa

Also use label to reference the data, note to add `$` to present the address itself:

    rsp:                    /* start of the inject code */
        movq $data, %rdi    /* padding address of the string to the first argument */
        /* ... */           /* jump to touch */

## phase 4 ##

The farm provide many rubbish instructions. However, if we jump to the middle of an instruction, it will be interpreted as a new one. Compare:

    401a44:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax

with:

    401a46: 48 89 c7                movq   %rax,%rdi
    401a49: c3                      retq

Since we have the control of the stack, the `retq` will pass control to another place we want.

Note that all the instructions are presented in the writeup.

The only way to transfer a constant in the inject code is using `popq`.

Then seek the farm to find a way to transfer it to the register you want(`$rdi`).

See details in `src/4.s`.

## phase 5 ##

It is said that the stack address is randomized, so we cannot use absolute addresses.

But we have the register `$rsp` which present the stack address. Seek the farm to transfer it to another register.

We also need to add/sub an offset to point it to the string, luckily we have `add_xy()` in the farm.

Note that the offset can be calculated using label.

Seek the farm to make a difficult transfer to the final one. (...)

See details in `src/5.s`.

> Note: in x86-64, `movl` will set the high 32-bit of the destination to all zero if it is a register. Moreover, any 32-bit instructions that modify the register will do so.
