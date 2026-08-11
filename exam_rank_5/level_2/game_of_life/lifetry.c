#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_info
{
	int		width;
	int		height;
	int		iterations;
	char	*board;
} t_info;

int	init_board(t_info *t, char **av)
{
	t->width = atoi(av[0]);
	t->height = atoi(av[1]);
	t->iterations = atoi(av[2]);
	if (t->width <= 0 || t->height <= 0 || t->iterations < 0)
		return (1);	
	t->board = calloc(t->width * t->height, 1);
	return (t->board == NULL);
}

int set_board(t_info *t)
{
	char buf;
	int flag = 0, x = 0, y = 0;
	while (read(STDIN_FILENO, &buf, 1))
	{
		if (buf == 'w' && x > 0)
			x--;
		else if (buf == 's' && x < t->height)
			x++;
		else if (buf == 'a' && y > 0)
			y--;
		else if (buf == 'd' && y < t->height)
			y++;
		else if (buf == 'x')
			flag = !flag;
		else if (buf != '\n')
			return (1);
		if (flag)
			t->board[x * t->width + y];
	}
	return (0);
}

int count_neighbours(t_info t, int x, int y)
{
	int n = 0, dx, dy, nx, ny;
	for (dx = -1; dx <= 1; dx++)
		for (dy = -1; dy <= 1; dy++)
		{
			nx = x + dx;
			ny = y + dy;
			if (nx > 0 && nx < t.width && ny > 0 && ny < t.height && t.board[nx * t.width + ny])
				n++;
		}
	return (n);	
}

void iterate_game(t_info *t)
{
	int neigboard, x, y;
	char *next = calloc(t->width * t->height, 1);
	for (x = 0; x < t->height; x++)
		for (y = 0; y < t->width; y++)
		{
			neigboard = count_neighbours(*t, x, y);
			next[x * t->width + y] = (neigboard == 3 || (t->board[x * t->width + y] && neigboard == 2));
		}
	free(t->board);
	t->board = next;
}

int main(int ac, char **av)
{
	t_info t;
	if (ac != 4)
		return (1);
	if (init_board(&t, &av[1]))
		return (2);
	if (set_board(&t))
		return (free(t.board), 3);
	for (int i = 0; i < t.iterations; i++)
		iterate_game(&t);
	
	return (0);
	
}