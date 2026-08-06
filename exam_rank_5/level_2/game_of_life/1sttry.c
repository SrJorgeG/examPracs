#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct s_struct
{
	int height;
	int width;
	int iterations;
	char *board;
} t_struct;

int set(t_struct *i)
{
	char buf;
	int x, y;

	while(read(STDIN_FILENO, &buf, 1) == 1)
	{
		if (buf == 'w' && x)
	}

}

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;
	t_struct i;
	i.height = atoi(av[1]);
	i.width = atoi(av[2]);
	i.iterations = atoi(av[3]);
	if (i.height <= 0 || i.width <= 0 || iterations < 0)
		return 2;
	i.board = calloc(width * height + 1, 1);
	if (!i.board)
		return 3;
	if(set(t_struct *i))
		return (free(i.board), 4);
}
