#include <unistd.h>
#include <stdio.h>

unsigned char   reverse_bits_better(unsigned char octet)
{
    int i = 0;
    unsigned char rev_bits = 0;
    while (i < 8)
    {
        rev_bits <<= 1;
        rev_bits |= (octet & 1);
        octet >>= 1;
        i++;
    }
    return (rev_bits);
}

unsigned char   reverse_bits(unsigned char octet)
{
    return (((octet & 0b00000001) << 7) |
            ((octet & 0b00000010) << 5) |
            ((octet & 0b00000100) << 3) |
            ((octet & 0b00001000) << 1) |
            ((octet & 0b00010000) >> 1) |
            ((octet & 0b00100000) >> 3) |
            ((octet & 0b01000000) >> 5) |
            ((octet & 0b10000000) >> 7));
}

void   printbits(unsigned char octet)
{
    unsigned char bits [8];
    int i = 0;
    while (i < 8)
    {
        bits[i] = ((octet >> (7 - i)) & 1) + '0';
        i++;
    }
    write(1, bits, 8);
}

int main()
{
    unsigned char   original = 0b01001101;
    unsigned char   rev;

    printbits(original);
    rev = reverse_bits(original);
    write(1, "\n", 1);
    printbits(rev);
    return 0;
}
