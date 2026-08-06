#include "life.h"

void print_board(t_life l)
{
	int x, y;
	for (x = 0; x < l.height; x++)
	{
		for (y = 0; y < l.width; y++)
			l.board[x * l.width + y] ? putchar('0') : putchar(' ');
		putchar('\n');
	}
}

int set_board(t_life *l)
{
	char buf;
	int flag = 0, x = 0, y = 0;
	
	while (read(STDIN_FILENO, &buf, 1) == 1)
	{
		if )
		{
			if (buf == 'w' && x > 0)
			{
				x--;
				if (flag)
					l->board[x * l->width + y] = '0';
			}
			else if (buf == 's' && x < l->height - 1)
			{
				x++;
				if (flag)
					l->board[x * l->width + y] = '0';
			}
			else if (buf == 'a' && y > 0)
			{
				y--;
				if (flag)
					l->board[x * l->width + y] = '0';
			}
	}
		else if (buf == 'd' && y < l->width - 1)
			y++;
			if (flag)
				l->board[x * l->width + y] = '0'; 
		else if (buf == 'x')
			flag = !flag;
		else if (buf != '\n' && buf != 'w' && buf != 's' && buf != 'a' && buf != 'd')
			return (1);
	}
	return (0);
}

int init_board(t_life *l, char **av)
{
	l->width = atoi(av[0]);
	l->height = atoi(av[1]);
	l->iterations = atoi(av[2]);
	if (l->width <= 0 || l->height <= 0 || l->iterations < 0)
		return (1);
	l->board = calloc(l->height * l->width + 1, sizeof(char));
	return (l->board == NULL);
}

int count_neighbors(t_life *l, int x, int y)
{
	int n = 0, dx, dy, nx, ny;

	for (dx = -1; dx <= 1; dx++)
		for (dy = -1; dy <= 1; dy++)
		{
			if (dx == 0 && dy == 0)
				continue;
			nx = x + dx;
			ny = y + dy;
			if (nx >= 0 && nx < l->height && ny >= 0 && ny < l->width && l->board[nx * l->width + ny] == '0')
				n++;
		}
	return (n);
}

void iterate_game(t_life *l)
{
	char *next;
	int neigboard;

	next = calloc(l->height * l->width +1, sizeof(char));
	for (int x = 0; x < l->height; x++)
		for (int y = 0; y < l->width; y++)
		{
			neigboard = count_neighbors(l, x, y);
			if (l->board[x * l->width + y] == '0')
				next[x * l->width + y] = (neigboard == 2 || neigboard == 3) ? '0' : ' ';
			else
				next[x * l->width + y] = (neigboard == 3) ? '0' : ' ';
		}
	free(l->board);
	l->board = next;
}

int main(int ac, char **av)
{
	t_life l;
	int i;
	if (ac != 4)
		return (1);
	if (init_board(&l, av + 1))
		return (2);
	if (set_board(&l))
		return (free(l.board), 3);
	for (i = 0; i < l.iterations; i++)
		iterate_game(&l);
	print_board(l);
	free(l.board);
	return (0);
}
