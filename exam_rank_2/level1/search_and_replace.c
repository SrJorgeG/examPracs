#include <unistd.h>

void	search_and_replace(char *str, char search, char replace)
{
	int	i = 0;

	while (str[i])
	{
		if (str[i] == search)
			str[i] = replace;
		write(1, &str[i], 1);
		i++;
	}
}

int main(int ac, char *av[])
{
	if (ac == 4)
		search_and_replace(av[1], av[2][0], av[3][0]);
	write(1, "\n", 1);	
	return 0;
}

// int main(int ac, char *av[])
// {
// 	int	i = 0;

// 	if (ac == 4)
// 	{
// 		while (av[1][i])
// 		{
// 			if (av[1][i] == av[2][0])
// 				av[1][i] = av[3][0];
// 			write(1, &av[1][i], 1);
// 			i++;
// 		}
// 	}
// 	write(1, "\n", 1);
// 	return 0;
// }
