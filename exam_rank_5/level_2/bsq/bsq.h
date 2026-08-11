#ifndef BSQ_H
# define BSQ_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_bsq {
  int	  height;
  int	  width;
  char	empty;
  char	obstacle;
  char	full;
  char	*board;
}	t_bsq;

#endif
