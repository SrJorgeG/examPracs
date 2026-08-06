#include "life.h"

/*
	1) Recorre el tablero final y lo imprime en stdout 
  2) Imprime un salto de linea al final de cada fila.
*/
void print_board(t_life l)
{
	int x;
	int y;
	for (x = 0; x < l.height; x++)
	{
		for (y = 0; y < l.width; y++)
      l.board[x * l.width + y] ? putchar('0') : putchar(' ');
		putchar('\n');
	}
}

/*
	1) arranca en la esquina superior izquierda (0,0)
  2) lee la entrada standard caracter a caracter 
	3) 'w','a','s','d' mueven el boli una casilla, respetando los	limites del tablero. 'x'mientras flag esta activo, cada movimiento deja una celula viva en la posicion actual.
	4) Cualquier caracter que no sea uno de los comandos validos se considera entrada invalida.
*/
int set_board(t_life *l)
{
	char buf;
	int flag = 0;
	int x = 0;
	int y = 0;
	
	while (read(STDIN_FILENO, &buf, 1) == 1)
	{
		if (buf == 'w' && x > 0)
		{
			x--; // CHANGED: mark moved inside this branch so it only fires on an actual 'w' move
			if (flag) // CHANGED: mark moved inside this branch so it only fires on an actual 'w' move
				l->board[x * l->width + y] = '0'; // CHANGED: mark moved inside this branch so it only fires on an actual 'w' move
		}
		else if (buf == 's' && x < l->height - 1)
		{
			x++; // CHANGED: mark moved inside this branch so it only fires on an actual 's' move
			if (flag) // CHANGED: mark moved inside this branch so it only fires on an actual 's' move
				l->board[x * l->width + y] = '0'; // CHANGED: mark moved inside this branch so it only fires on an actual 's' move
		}
		else if (buf == 'a' && y > 0)
		{
			y--; // CHANGED: mark moved inside this branch so it only fires on an actual 'a' move
			if (flag) // CHANGED: mark moved inside this branch so it only fires on an actual 'a' move
				l->board[x * l->width + y] = '0'; // CHANGED: mark moved inside this branch so it only fires on an actual 'a' move
		}
		else if (buf == 'd' && y < l->width - 1)
		{
			y++; // CHANGED: mark moved inside this branch so it only fires on an actual 'd' move
			if (flag) // CHANGED: mark moved inside this branch so it only fires on an actual 'd' move
				l->board[x * l->width + y] = '0'; // CHANGED: mark moved inside this branch so it only fires on an actual 'd' move
		}
		else if (buf == 'x')
			flag = !flag;
		else if (buf != '\n' && buf != 'w' && buf != 's' && buf != 'a' && buf != 'd')
			return (1);
		// CHANGED: removed the unconditional "if (flag) l->board[x*l->width+y] = '0';" that used to sit here,
		// since it wrongly marked the cell on 'x' toggles and on '\n' as well, not only on real moves.
	}
	return (0);
}

/*
	1) Parsea (width, height,	iterations recibidos  
  2) valida que las dimensiones sean estrictamente positivas y que iterations no sea negativo. 
	3) Si los valores son validos, alloca el tablero 
*/
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

/*
	1) Cuenta cuantas de las 8 celulas vecinas  dx,dy en {-1,0,1}, saltando el caso (0,0)
	2) Toda celula fuera de los limites del tablero se considera muerta asi que no suma
	3) Devuelve el numero total de vecinos vivos, entre 0 y 8.
*/
int count_neighbors(t_life *l, int x, int y)
{
	int n;
	int dx;
	int dy;
	int nx;
	int ny;
	n = 0;

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

/*
  1) allocar un tablero nuevo
  2) Recorrer el tablero viejo contando vecinos y escribiendo el resulado en el tablero nuevo
  3) liberar el tablero viejo
  4) asignar el tablero nuevo a la struct
*/
void iterate_game(t_life *l)
{
	char *next;
	int neigboard;

	next = calloc(l->height * l->width +1, sizeof(char));
	for (int x = 0; x < l->height; x++)
	{
		for (int y = 0; y < l->width; y++)
		{
			neigboard = count_neighbors(l, x, y);
			if (l->board[x * l->width + y] == '0')
				next[x * l->width + y] = (neigboard == 2 || neigboard == 3) ? '0' : ' ';
			else
				next[x * l->width + y] = (neigboard == 3) ? '0' : ' ';
		}
	}
	free(l->board);
	l->board = next;
}

/*
	  1) validar que se reciban exactamente los 3 argumentos.
	  2) inicializar el tablero vacio con las dimensiones pedidas.
	  3) leer stdin y dibujar el estado inicial con el boli.
	  4) simular tantas generaciones como indique "iterations".
    5) finalmente imprime el resultado.
*/
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
