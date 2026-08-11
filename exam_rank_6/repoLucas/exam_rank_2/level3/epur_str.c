#include <unistd.h>

void	epur_str(char *str)
{
	int	i;
	int	aux;

	i = 0;
	aux = 0;
		while (str[i] == ' ' || str[i] == '	')
			i++;

		while (str[i])
		{
			if (str[i] == ' ' || str[i] == '	')
				aux = 1;
			if (!(str[i] == ' ' || str[i] == '	'))
			{
				if (aux)
					write(1, " ", 1);
				aux = 0;
				write(1, &str[i], 1);
			}
			i++;
		}
}

int main (int ac, char **av)
{
	if (ac == 2)
		epur_str(av[1]);
	write(1, "\n", 1);
	return 0;
}
