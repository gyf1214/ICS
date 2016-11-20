# HW 6 #

## 7.8 ##

### A ###

- REF(main.1) -> DEF(main.1)
- REF(main.2) -> DEF(main.2)

Since `main` in Module 2 is a local symbol, so it won't conflict with main in Module 1.

### B ###

UNKNOWN

Since `x` in both two modules are weak, `ld` will pick any one.

Some linkers may pick the one with bigger size.

### C ###

ERROR

Since `x` in both two modules are strong, it causes a conflict.

## 7.9 ##

`main` in `bar6.c` is a weak symbol and in `foo6.c` is a strong one (a function), so even though the two symbols are of different type, `main` in `bar6.c` will be replaced by the strong one in `foo6.c`. So actually `main` in `bar6.c` refer to the start address of `main` function in `foo6.c`, and since its type is char, it will be the first byte of the codes of `main` function.

## 7.12 ##

### A ###

0x4004f8 - (0x4004e0 + 0xa) - 4 = 0xa

### B ###

0x400500 - (0x4004d0 + 0xa) - 4 = 0x22

## 7.13 ##

### A ###

I don't have these files on my system (Mac OS X), so I test on `ics9.pku.edu.cn`.

Use command `ar t <file>.a | wc -l`.

There are 3 version of `libc.a` & `libm.a` of different architecture:

libc.a

- /usr/lib32/libc.a                 1605 files
- /usr/libx32/libc.a                1576 files
- /usr/lib/x86_64-linux-gnu/libc.a  1579 files

libm.a

- /usr/lib32/libm.a                 439 files
- /usr/libx32/libm.a                471 files
- /usr/lib/x86_64-linux-gnu/libm.a  471 files

### B ###

First, `-Og` specifies an optimization level, it is between `-O0` and `-O1`. Second, `-g` indicates including debug info in the object file. Only `-Og` will affect the generated code, and `-g` will affect the generated object file. So the generated codes (`.text` section) are the same with `-Og` and `-Og -g`, but the object files are different.

### C ###

My system is Mac OS X, so the object file type is not ELF by Mach-o. I first test on `ics9.pku.edu.cn`:

`ldd /usr/bin/gcc`:

- linux-vdso.so.1
- libc.so.6
- ld-linux-x86-64.so.2

`readelf -dl /usr/bin/gcc`, Dynamic section:

- Shared library: [libc.so.6]
- Shared library: [ld-linux-x86-64.so.2]
- ...

The first one is the shared library for loading libraries on linux. It is not included in the Dynamic section.

On my system (Mac OS X), there's no `gcc` compiler, but `clang`. And the tool to check object files is `otool`:

`otool -L /usr/bin/clang`:

- /usr/lib/libxcselect.dylib (compatibility version 1.0.0, current version 1.0.0)
- /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1213.0.0)

`dylib` is BSD style dynamic libraries and darwin, kernel of OS X, is based on BSD.
