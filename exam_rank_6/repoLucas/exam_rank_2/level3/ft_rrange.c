#include <stdlib.h>
#include <stdio.h>

int     *ft_rrange(int start, int end) {
    int i = 0;
    int len = (end - start) + 1;

    int *res = (int *)malloc(sizeof(int) * len);
    if (!res)
        return NULL;
    while (i < len)
    {
        if (end > start)
            res[i++] = start++;
        else
            res[i++] = start--;
    }
    return res;
}

int main() {
    int start = 1;
    int end = 3;
    int *tab = ft_rrange(start, end);
    for (int i = 0; i <= end; i++)
        printf("tab[%i] = %i\n", i, tab[i]);
    return 0;
}