#include <unistd.h>

void    rotone(char *str)
{
    int i = 0;

    while (str[i])
	{
		if ((str[i] >= 'A' && str[i] <= 'Y') || (str[i] >= 'a' && str[i] <= 'y'))
			str[i] += 1;
		else if (str[i] == 'Z' || str[i] == 'z')
			str[i] -= 25;
		write(1, &str[i], 1);
		i++;
	}
}
int main(int ac, char *av[])
{
	if (ac == 2)
		rotone(av[1]);
	write(1, "\n", 1);
	return 0;
}

int main(int ac, char *av[])
{
	int i = 0;

	if (ac == 2)
	{
		while (av[1][i])
		{
			if ((av[1][i] >= 'a' && av[1][i] <= 'y') || (av[1][i] >= 'A' && av[1][i] <= 'Y'))
				av[1][i] += 1;
			else if (av[1][i] == 'z' || av[1][i] == 'Z')
				av[1][i] -= 25;
			write(1, &av[1][i], 1);
			i++;
		}
	}
	write(1, "\n", 1);
	return 0;
}
