#include <stdio.h>

int	ft_atoi(const char *str)
{
	int i = 0;
	int	sign = 1;
	int	res = 0;

	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		while (str[i] == '-' || str[i] == '+')
			i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (sign * res);
}

int main(int ac, char *av[])
{
	printf("%d", ft_atoi(av[1]));
	return 0;
}
