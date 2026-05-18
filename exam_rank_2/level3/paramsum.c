#include <unistd.h>

void putnbr(int n) {
    if (n == -2147483648) {
        write(1, "-", 1);
        write(1, "2", 1);
        n = 147483648;
    }
    if (n < 0) {
        write(1, "-", 1);
        n *= -1;
    }
    if (n >= 10) {
        putnbr(n/10);
        putnbr(n%10);
    }
    else {
        n += '0';
        write(1, &n, 1);
    }
}

int main(int ac, char **av) {
    (void)av;
    if (ac > 1) putnbr(ac - 1);
    else write(1,"0",1);
    write(1, "\n", 1);
    return 0;
}