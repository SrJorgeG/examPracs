#include <unistd.h>
void	snake_to_camel(char *str)
{
	int	i = 0;

	while(str[i])
	{
		if (str[i] == '_')
		{
			i++;
			str[i] -= 32;
		}
		write(1, &str[i], 1);
		i++;
	}
}
int main(int ac, char *av[])
{
	if (ac == 2)
		snake_to_camel(av[1]);
	write(1, "\n", 1);
	return 0;
}

// int main(int ac, char **av)
// {
// 	int i = 0;

// 	if (ac == 2)
// 	{
// 		while (av[1][i])
// 		{
// 			if (av[1][i] == '_')
// 			{
// 				i++;
// 				av[1][i] -= 32;
// 			}
// 			write (1, &av[1][i], 1);
// 			i++;
// 		}
// 	}
// 	write (1, "\n", 1);
// }