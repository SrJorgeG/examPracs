#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_life
{
	int height;
	int width;
	int iterations;
	char *board;
} t_life;

void	print_board(t_life t)
{
	for (int x = 0; x < t.height; x++)
	{
		for (int y = 0; y < t.width; y++)
			putchar((t.board[x * t.width + y] ? '0' : ' '));
		putchar('\n');
	}
}

int init_board(t_life *t, char **av)
{
	t->height = atoi(av[0]);
	t->width = atoi(av[1]);
	t->iterations = atoi(av[2]);
	if (t->height <= 0 || t->width <= 0 || t->iterations < 0)
		return 1;
	t->board = calloc(t->width * t->height, 1);
	return (t->board == NULL);
}

int set(t_life *t)
{
	char buf;
	int flag = 0, x = 0, y = 0;
	while(read(STDIN_FILENO, &buf, 1) == 1)
	{
		if (buf == 'w' && x > 0)
			x--;
		else if (buf == 's' && x < t->height - 1)
			x++;
		else if (buf == 'a' && y > 0)
			y--;
		else if (buf == 'd' && y < t->width - 1)
			y++;
		else if (buf == 'x')
			flag = !flag;
		else if (buf != '\n')
			return (1);
		if (flag)
			t->board[x * t->width + y] = 1;
	}
	return (0);
}

int count_neighbours(t_life t, int x, int y)
{
	int nx, ny, dx, dy, n = 0;
	for (dx = -1; dx <= 1; dx++)
		for (dy = -1; dy <= 1; dy++)
		{
			if (dx == 0 && dy == 0)
				continue;
			nx = x + dx;
			ny = y + dy;
			if (nx >= 0 && nx < t.height && ny >= 0 && ny < t.width && t.board[nx * t.width + ny])
				n++;
		}
	return n;
}	

void iterate_game(t_life *t)
{
	int x, y, nigboard;
	char *next = calloc(t->width * t->height, 1);
	for (x = 0; x < t->height; x++)
		for (int y = 0; y < t->width; y++)
		{
			nigboard = count_neighbours(*t, x, y);
			next[x * t->width + y] = (nigboard == 3 || (t->board[x * t->width + y] && nigboard == 2));
		}
	free(t->board);
	t->board = next;
}

int main(int ac, char **av)
{
	t_life t;
	if (ac != 4)
		return 1;
	if (init_board(&t, av + 1))
		return 2;
	if (set(&t))
		return (free(t.board), 3);
	for (int i = 0; i < t.iterations; i++)
		iterate_game(&t);
	print_board(t);
	free(t.board);
	return (0);		
}
