# Homework 2 #

## 2.86 ##

| Description                         | Value                  | Decimal      |
|:----------------------------------- |:----------------------:|:------------:|
| Smallest positive denormalized      | 0x00000000000000000001 | 10^-4950     |
| Smallest positive normalized        | 0x00018000000000000000 | 10^-4931     |
| Largest normalized                  | 0x7FFEFFFFFFFFFFFFFFFF | 10^4932      |

## 2.95 ##

```c++
//copy from lab-1

unsigned float_half(unsigned uf) {
    unsigned expo = (uf >> 23) & 0xFF;
    if (expo == 0xFF) {
        return uf;
    } else if (expo <= 1) {
        unsigned mask = (1 << 23) - 1;
        unsigned frac = uf & mask;
        unsigned ffrac = (frac >> 1) | (expo << 22);
        ffrac = ffrac + ((frac & ffrac) & 1);
        return (uf & (1 << 31)) | ffrac;
    } else {
        return uf - (1 << 23);
    }
}
```

## 2.97 ##

```c++
//copy from lab-1

unsigned float_i2f(int x) {
    unsigned ret = 0;
    unsigned expo = 0;
    unsigned frac, ffrac;
    int t = 16;
    if (x < 0) {
        if (x == 0x80000000) return 0xcf000000;
        x = -x;
        ret = ret | (1 << 31);
    } else if (!x) {
        return 0;
    }
    frac = x;
    while (t) {
        if (x >> t) {
            expo += t;
            x = x >> t;
        }
        t = t >> 1;
    }
    frac = frac << (32 - expo);
    ffrac = frac >> 9;
    frac = frac & 0x1FF;
    expo = expo + 0x7F;
    ret = (ret | ffrac) | (expo << 23);
    if (frac > 0x100 || (frac == 0x100 && (ffrac & 1))) ++ret;
    return ret;
}
```
