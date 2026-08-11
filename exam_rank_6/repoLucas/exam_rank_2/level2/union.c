#include <unistd.h>
#include <stdbool.h>

void	union_ting(char *str1, char *str2)
{
	int	i = 0;
	bool	seen[256] = {false};

	while (str1[i])
	{
		if (!seen[str1[i]])
		{
			write(1, &str1[i], 1);
			seen[str1[i]] = 1; 
		}
		i++;
	}
	i = 0;
	while (str2[i])
	{
		if (!seen[str2[i]])
		{
			write(1, &str2[i], 1);
			seen[str2[i]] = 1; 
		}
		i++;
	}
}

int main(int ac, char *av[])
{
	if (ac == 3)
		union_ting(av[1], av[2]);
	write(1, "\n", 1);	
	return 0;
}

int main(int ac, char *av[])
{
    int i = 0;
    bool    seen[256] = {false};

    if (ac == 3)
    {
        while (av[1][i])
        {
            if (!seen[av[1][i]])
            {
                write(1, &av[1][i], 1);
                seen[av[1][i]] = 1;
            }
            i++;
        }
        i = 0;
        while (av[2][i])
        {
            if (!seen[av[2][i]])
            {
                write(1, &av[2][i], 1);
                seen[av[2][i]] = 1;
            }
            i++;
        }
    }
    write(1, "\n", 1);
    return 0;
}