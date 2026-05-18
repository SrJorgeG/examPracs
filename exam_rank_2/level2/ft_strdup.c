#include <stdio.h>
#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i++]);
	return (i);
}
char    *ft_strdup(char *src)
{
	char	*dup;
	int	i = 0;

	dup = (char *)malloc(ft_strlen(src) + 1);
	if (!dup)
		return (NULL);
	while (src[i])
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int main(int ac, char *av[])
{
	printf("%s\n", ft_strdup(av[1]));
	return 0;
}
