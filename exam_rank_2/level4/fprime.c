#include <stdlib.h>
#include <stdio.h>

int main(int ac, char **av) {
    if (ac == 2) {
        if (!*av[1] || av[1][0] == '-') return printf("\n");
        int n = atoi(av[1]);
        if (n == 1) printf("1");
        
        int i = 1;
        while(n >= ++i) {
            if (n % i == 0) {
                printf("%d", i);
                if (n == i) break;
                printf("*");
                n /= i;
                i = 1;
            }
        }
    }
    printf("\n");
    return 0;
}