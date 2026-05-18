#include <unistd.h>
void	last_word(char *str)
{
	int	i = 0;
	int	end;

	while(str[i])
		i++;
	i--; //'\0'
	while (i >= 0 && (str[i] == ' ' || str[i] == '\t'))
		i--;
	if (i < 0)
		return /*(write(1, "\n", 1), 0)*/;
	end = i;
	while (i >= 0 && str[i] != ' ' && str[i] != '\t')
		i--;
	i++; //' '
	while (i <= end)
		write(1, &str[i++], 1);
}

int main(int ac, char *av[])
{
	if (ac == 2)
		last_word(av[1]);
	write(1, "\n", 1);
	return 0;
}

// int	main(int ac, char **av)
// {
// 	int i = 0;
// 	int end;
// 	int start;

// 	if (ac == 2)
// 	{
// 		while (av[1][i])
// 			i++;
// 		i--;
// 		while (i >= 0 && (av[1][i] == ' ' || av[1][i] == '\t'))
// 			i--;
// 		if (i < 0)
// 			return (write(1, "\n", 1), 0);
// 		end = i;
// 		while (i >= 0 && av[1][i] != '\t' && av[1][i] != ' ')
// 			i--;
// 		start = i + 1;
// 		while (start <= end)
// 		{
// 			write (1, &av[1][start], 1);
// 			start++;
// 		}
// 	}
// 	write (1, "\n", 1);
// }

// int	main(int ac, char **av)
// {
// 	int i, end, start;

// 	if (ac == 2)
// 	{
// 		i = 0;
// 		// Skip to end of string
// 		while (av[1][i])
// 			i++;

// 		// Skip trailing spaces/tabs
// 		i--;
// 		while (i >= 0 && (av[1][i] == ' ' || av[1][i] == '\t'))
// 			i--;

// 		if (i < 0) // No word found
// 			return (write(1, "\n", 1));

// 		end = i;

// 		// Move backward to find beginning of the last word
// 		while (i >= 0 && av[1][i] != ' ' && av[1][i] != '\t')
// 			i--;

// 		start = i + 1;

// 		// Print the last word
// 		while (start <= end)
// 			write(1, &av[1][start++], 1);
// 	}
// 	write(1, "\n", 1);
// 	return (0);
// }


// int main(int ac, char **av) {
//     if (ac == 2) {
//         int i = -1;
//         int j = 0;
//         while(av[1][++i]) {
//             if (av[1][i] == 32 && av[1][i+1] >= 33 && av[1][i+1] <= 126)
//                 j = i+1;
//         }
//         while (av[1][j] >= 33 && av[1][j] <= 127) {
//             write(1, &av[1][j++], 1);
//         }
//     }
//     write(1, "\n", 1);
//     return 0;
// }