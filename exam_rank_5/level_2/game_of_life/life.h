#ifndef LIFE_H
# define LIFE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_life {
	int height;
	int width;
	int iterations;
	char *board;
} t_life;

#endif
