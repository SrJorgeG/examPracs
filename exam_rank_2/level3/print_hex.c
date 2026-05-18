#include <unistd.h>

#define HEX "0123456789abcdef"

void putnbr_base(int n) {
    if (n/16 == 0)
        write(1, &HEX[n % 16], 1);
    else {
        putnbr_base(n / 16);
        write(1, &HEX[n % 16], 1);
    }
}

int is_space(char c) {
    if (c == ' ' || (c >= 9 && c <= 13)) return 1;
    return 0;
}

int ft_atoi(char *s) {
    int res = 0;
    int i = 0;
    while(is_space(s[i]))
        i++;
    if (s[i] == '+')
        i++;
    while(s[i] >= '0' && s[i] <= '9') {
        res = res * 10 + s[i] - '0';
        i++;
    }
    return res;
}

int main(int ac, char **av) {
    if (ac == 2) {
        if (!*av[1] || av[1][0] == '-')
            return write(1, "\n", 1);
        putnbr_base(ft_atoi(av[1]));
    }
    write(1, "\n", 1);
    return 0;
}