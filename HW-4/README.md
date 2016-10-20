# HW 4 #

## How to build & test Y86-64 code ##

- Download Y86-64 tools on <http://csapp.cs.cmu.edu/>
- Extract `sim.tar` and copy folder `sim/` to this folder
- `make all` to build and `make test-all` to test all the `.ys` & `.hcl` codes

## 4.47 ##

Translating C code from array to pointer is trivial

There are some problems when translating to Y86-64

First, since Y86-64 don't have shift, multiply or `lea` instructions, we can only use multiple `addq` to calculate Count * 8 (in pointer calculating)

Second, since Y86-64 don't have `test` or `cmp` instructions, comparisons of two numbers are complex. But we can use `andq` to compare with 0

Third, there are too few registers (...)

Fortunately these problems are easy to solve

## 4.49 ##

To use cmovX to replace the conditional swap, an idea is to use cmovX to get the large one of two in one register, and the other one in another, and then put them back to memory

Get the larger one is trivial, but how to get __THE OTHER ONE__?

We know that `xor` satisfy `x == (x ^ y) ^ y`. So we can put `x ^ y` in one register (for instance %rcx), then use cmovX to get the larger one of x & y (in %r11), finally use `xorq %r11, %rcx` to get another of x & y in %rcx

## 4.52 ##

Trivial, if you can read the original HCL code, only to notice that `iaddq` sets CC

> The original seq-full of Y86-64 version is available also on <http://csapp.cs.cmu.edu/>
