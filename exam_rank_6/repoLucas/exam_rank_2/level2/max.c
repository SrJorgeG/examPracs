#include <stdio.h>

int max(int *tab, unsigned int len)
{
    unsigned int i = 0;
    int max = tab[0];
    while (i < len) 
	{
        if (tab[i] > max)
            max = tab[i];
		i++;
    }
    return max;
}
int main(void)
{
    int nums[] = {5, 8, 2, 14, 6};
    printf("Max is: %d\n", max(nums, 5));  // should print 14
    return 0;
}