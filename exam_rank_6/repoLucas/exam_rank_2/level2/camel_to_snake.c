#include <unistd.h>

void	camel_to_snake(char *str)
{
	int	i = 0;

	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			write(1, "_", 1);
			str[i] += 32;
		}
		if (str[i] >= 'a' && str[i] <= 'z')
			write(1, &str[i], 1);
		i++;
	}
}
int main(int ac, char *av[])
{
	if (ac == 2)
		camel_to_snake(av[1]);
	write(1, "\n", 1);
	return 0;
}

// int main(int ac, char *av[])
// {
// 	int	i = 0;

// 	if (ac == 2)
// 	{
// 		while(av[1][i])
// 		{
// 			if (av[1][i] >= 'A' && av[1][i] <= 'Z')
// 			{
// 				write(1, "_", 1);
// 				av[1][i] += 32;
// 			}
// 			if (av[1][i] >= 'a' && av[1][i] <= 'z')
// 				write(1, &av[1][i], 1);
// 			i++;
// 		}
// 	}
// 	write(1, "\n", 1);
// 	return 0;
// }
