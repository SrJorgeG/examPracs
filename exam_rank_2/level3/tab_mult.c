int	ft_atoi(const char *str)
{
    int result = 0;
    // int sign = 1;
    int i = 0;

    // while (str[i] == ' ' || str[i] == '\n' || str[i] == '\r' || str[i] == '\v' ||str[i] == '\t')
    //     i++;
    // if (str[i] == '-' || str[i] == '+')
    // {
    //     if (str[i] == '-')
    //         sign *= -1;
    //     while (str[i] == '-' || str[i] == '+')
    //         i++;
    // }
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result);
}

void    ft_putnbr(int n)
{
    char    *str = "0123456789";

    if (n >= 10)
        ft_putnbr(n / 10);
    n = n % 10;
    write(1, &str[n], 1);
}

int main(int ac, char *av[])
{
    int num = 0;
    int i = 1;

    if (ac == 2)
    {
        num = ft_atoi(av[1]);
        while (i <= 9)
        {
            ft_putnbr(i);
            write(1, " x ", 3);
            ft_putnbr(num);
            write(1, " = ", 3);
            ft_putnbr(i * num);
            write(1, "\n", 1);
            i++;
        }
    }
    else
        write(1, "\n", 1);
    return 0;
}