# HW3 #

## 3.60 ##

### A ###

x in %rdi

n in %esi & %ecx

result in %rax

mask in %rdx

%r8 is the temporary register of expression (x & mask)

### B ###

result = 0

mask = 1

### C ###

mask != 0

### D ###

mask = mask << n

### E ###

result |= x & mask

### F ###

```C++
long loop(long x, int n) {
    long result = 0;
    long mask;
    for (mask = 1; mask !=0; mask = mask << n) {
        result |= x & mask;
    }
    return result;
}
```

## 3.61 ##

```C++
long cread_alt(long *xp) {
    long t = 0;
    xp = xp ? xp : &t;
    return *xp;
}
```

## 3.64 ##

First translate assembly code into C++:

```C++
long store_ele(long i, long j, long k, long *dest) {
    long rax = 13 * j;
    long rdi = 65 * i + rax;
    long rdx = k + rdi;
    *dest = *(A + rdx);
    return 3640;
}
```

So:

- T = 13;
- S * T = 65;
- 8 * R * S * T = 3640.

So:

- R = 7;
- S = 5;
- T = 13.

## 3.68 ##

We know offsets:

- str2::t -> 8;
- str2::u -> 32;
- str1::y -> 184.

Considering padding, we know that:

- 4 < B <= 8;
- 24 < 8 + 4 + 2 * A <= 32;
- 176 < 4 * A * B <= 184.

Solve the inequalities:

- A = 9;
- B = 5.

## 3.69 ##

First translate all pointers to numbers:

```C++
void test(long i, long bp) {
    int ecx = *(int *)(0x120 + bp);
    ecx += *(int *)bp;
    long rax = bp + 40 * i;
    long rdx = *(long *)(rax + 8);
    *(long *)(rax + 0x10 + rdx * 8) = ecx;
}
```

We know that bp + 8 is the start address of bp -> a, so rax + 8 is the address of bp -> a[i], and sizeof(a_struct) = 40.

Then consider the first statement. We know that CNT * sizeof(a_struct) = 0x120 - 8, so CNT = 7.

Then consider the last two statements, rdx is a temporary register of ap -> idx, so the address of ap -> idx is rax + 8, and the offset of idx is 0. Therefore, idx is the first field of a_struct, and x is the second field of a_struct, which can also be validated by the last statement.

Assume M is the array size of a_struct::x, and sizeof(a_struct) = 8 + 8 * M. So M = 4.

The final answer in C++ code:

```C++
#define M 4
#define CNT 7

typedef struct {
    long idx;
    long x[M];
}a_struct;

typedef struct {
    int first;
    a_struct a[CNT];
    int last
}b_struct;

void test(long i, b_struct *bp) {
    int n = bp -> first + bp -> last;
    a_struct *ap = &bp -> a[i];
    ap -> x[ap -> idx] = n;
}
```
