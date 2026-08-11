#include <stdio.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
    int value1 = 0;
    int value2 = 0;
    int count = 0;
    int fin_num = 0;

    if (ac == 3)
    {
        value1 = atoi(av[1]);
        value2 = atoi(av[2]);
        while (count <= value2 && count <= value1)
        {
            if (value1 % count == 0 && value2 % count == 0)
                fin_num = count;
            count++;
        }
        printf("%d", fin_num);
    }
    printf("\n");
    return 0;
}