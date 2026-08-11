#include "bsq.h"

void print_board(t_bsq b)
{
  for (int i = 0; i < b.height; i++)
    fprintf(stdout, "%.*s\n", b.width, b.board + i * b.width);
}

size_t ft_strlen(char *s)
{
  const char *p = s;
  while (*p)
    p++;
  return (p - s);
}

int min3(int a, int b, int c)
{
	int m = a < b ? a : b;
	m = m < c ? m : c;
	return m;
}

int solve(t_bsq *b)
{
	int best_row = 0, best_col = 0, best_size = 0;
	int *dp = (int*)calloc(b->height * b->width, sizeof(int));
	if (!dp)
		return 1;
	for (int x = 0; x < b->height; x++) 
		for (int y = 0; y < b->width; y++) 
		{
			if (b->board[x * b->width + y] == b->obstacle)
				continue;
			if (x == 0 || y == 0)
				dp[x * b->width + y] = 1;
			else 
			{
				int up = dp[(x - 1) * b->width + y];
				int left = dp[x * b->width + (y - 1)];
				int diag = dp[(x - 1) * b->width + (y - 1)];
				dp[x * b->width + y] = 1 + min3(up, left, diag);
			}
			if (dp[x * b->width + y] > best_size) 
			{
				best_size = dp[x * b->width + y];
				best_row = x;
				best_col = y;
			}
		}
	free(dp);
	for (int x = best_row - best_size + 1; x <= best_row; x++)
	  for (int y = best_col - best_size + 1; y <= best_col; y++)
	    b->board[x * b->width + y] = b->full;
	return 0;
}

int proccess_file(FILE *f, t_bsq *b)
{
  char *line = NULL;
  ssize_t bytes;
  size_t buf = 0;

  if (fscanf(f, "%d %c %c %c ", &b->height, &b->empty, &b->obstacle, &b->full) != 4)
    return 1;
  b->board = NULL;
  if ( b->height <= 0 || b->empty == b->obstacle || b->empty == b->full || b->obstacle == b->full)
    return 1;
  for (int x = 0; x < b->height; x++) 
  {
    if ((bytes = getline(&line, &buf, f)) == -1)
      return (free(line), 1);
    int len = ft_strlen(line);
    if (len > 0 && line[len - 1] == '\n')
      len--;
    if (x == 0)
	{
      b->width = len;
      if (b->width <= 0)
        return (free(line), 1);
      b->board = (char*)calloc((b->height * b->width + 1), sizeof(char));
      if (!b->board)
        return (free(line), 1);
    }
    else if (len != b->width)
      return (free(line), 1);
    for (int y = 0; y < b->width; y++)
      b->board[x * b->width + y] = line[y];
  }
  free(line);
  for (int y = 0; y < b->height * b->width; y++)
    if (b->board[y] != b->empty && b->board[y] != b->obstacle)
      return (1);
  return 0;
}

int main(int ac, char** av)
{
	t_bsq b = {0};
	FILE *f = NULL;
	int i = 1;
	do
	{
		f = (ac == 1) ? STDIN_FILENO : fopen(av[i], "r");
		if (!f)
			return (fprintf(stderr, "Error: \n"), 1);
		if(proccess_file(f, &b))
			return (fprintf(stderr, "Error: \n"), free(b.board),  1);
		if (solve(&b))
			return (fprintf(stderr, "Error: \n"), free(b.board),  1);
		print_board(b);
		free(b.board);
		if (f && ac != 1)
			fclose(f);
	} while (i++ < ac);
	return 0;
}
