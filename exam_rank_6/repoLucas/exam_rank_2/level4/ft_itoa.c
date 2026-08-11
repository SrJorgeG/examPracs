#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

char	*ft_itoa(int nbr)
{
    char    *result;
    int     len = 0;
    int     sign = 0;
    int     n = 0;

    n = nbr;
    if (n == -2147483648)
        return("-2147483648\0");
    if (n <= 0)
    {
        len++;
        sign = 1;
    }
    while (n)
    {    
        n /= 10;
        len++;
    }
    if (!(result = malloc((len + 1) * sizeof(char))))
        return (NULL);

    if (nbr == 0)
    {
        result[0] = '0';  
        return result;
    }
    result[len] = '\0';
    if (sign)
    {    
        result[0] = '-';
        nbr = -nbr;
    }
    while (nbr)
    {
        result[--len] = (nbr % 10) + '0';
        nbr /= 10;
    }
    return (result);
}