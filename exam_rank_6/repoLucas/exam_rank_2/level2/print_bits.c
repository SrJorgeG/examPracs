#include <unistd.h>

void   print_bits(unsigned char octet)
{
    unsigned char bits [8] =
    {
        ((octet & 0b10000000) >> 7) + '0',
        ((octet & 0b01000000) >> 6) + '0',
        ((octet & 0b00100000) >> 5) + '0',
        ((octet & 0b00010000) >> 4) + '0',
        ((octet & 0b00001000) >> 3) + '0',
        ((octet & 0b00000100) >> 2) + '0',
        ((octet & 0b00000010) >> 1) + '0',
        ((octet & 0b00000001) >> 0) + '0'
    };
    write(1, bits, 8);
}

void   print_bits_better(unsigned char octet)
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

void    print_bits2(unsigned char octet)
{
    int i = 0;
    unsigned char   bits[8];

    while (i < 8)
    {
        bits[i] = (octet >> (7 - i) & 1) + '0';
        i++;
    }
    write(1, bits, 8);
}

unsigned char    reverse_bits(unsigned char octet)
{
    unsigned char   bits = 0;
    int i = 0;

    while (i > 8)
    {
        bits <<= 1;
        bits |= octet & 1;
        octet >>=1;
        i++;
    }
}
int main(void)
{
    unsigned char   num = 0b00110010;
    print_bits(num);
    write(1, "\n", 1);
    print_bits_better(num);
    write(1, "\n", 1);
    print_bits2(num);

    return 0;
}
