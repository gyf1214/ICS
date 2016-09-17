#include <stdio.h>

unsigned replace_byte(unsigned x, int i, unsigned char b) {
    return (x & (~(0xFF << (8 * i)))) | (b << (8 * i));
}

int main() {
    printf("0x%X\n", replace_byte(0x12345678, 2, 0xAB));
    printf("0x%X\n", replace_byte(0x12345678, 0, 0xAB));
    return 0;
}
