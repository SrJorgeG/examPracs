#include <stdio.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
	int	num1;
	int	num2;

	if (ac == 4)
	{
		num1 = ft_atoi(av[1]);
		num2 = ft_atoi(av[3]);
		if (av[2][0] == '+')
			printf("%d", num1 + num2);
		else if (av[2][0] == '-')
			printf("%d", num1 - num2);
		else if (av[2][0] == '*')
			printf("%d", num1 * num2);
		else if (av[2][0] == '/')
			printf("%d", num1 / num2);
		else if (av[2][0] == '%')
			printf("%d", num1 % num2);
	}
	printf("\n");
	return 0;
}
