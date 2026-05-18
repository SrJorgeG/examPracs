#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// Nombre de la tarea:    picoshell
// Archivos esperados:        picoshell.c
// Funciones permitidas:    close, fork, wait, exit, execvp, dup2, pipe
// ___________________________________________________________________

// Escriba la siguiente función:

// int    picoshell(char **cmds[]);

// El objetivo de esta función es ejecutar una tubería. Debe ejecutar cada uno de los
// comandos de cmds y conectar la salida de uno a la entrada del
// siguiente comando (igual que un shell).
// e
// Cmds contiene una lista terminada en nulo de comandos válidos. Cada fila
// de cmds es una matriz argv directamente utilizable para una llamada a execvp. El primer
// argumento de cada comando es el nombre o la ruta del comando y se puede pasar
// directamente como primer argumento de execvp.

// Si se produce algún error, la función debe devolver 1 (por supuesto, debes
// cerrar todos los fds abiertos antes). De lo contrario, la función debe esperar a todos los procesos hijos
// y devolver 0. En este directorio encontrarás un archivo main.c que
// contiene algo que te ayudará a probar tu función.


// Ejemplos: 
// ./picoshell /bin/ls «|» /usr/bin/grep picoshell
// picoshell
// ./picoshell echo “squalala” «|» cat «|» sed “s/a/b/g”
// squblblb


int picoshell(char **cmds[])
{
	int fd[2];
	int prev_fd = -1;
	int status;
	int i = 0;
	pid_t pid;

	if (!cmds[0] || !cmds[0][0])
		return 1;
	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fd) == -1)
			return 1;
		pid = fork();
		if (pid == -1)
		{
			if (cmds[i + 1])
			{
				close(fd[0]);
				close(fd[1]);
			}
			return 1;
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					exit(1);
				close(prev_fd);
			}
			if (cmds[i + 1])
			{
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) == -1)
					exit(1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		i++;
	}
	while (wait(&status) > 0)
		;
	return 0;
}

int picoshell2(char **cmds[])
{
	int	fd[2];
	int	status;
	int	i = 0;
	int	prev_fd = -1;
	pid_t pid;

	if (!cmds[0] || !cmds[0][0])
		return (1);
	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fd) == -1)
			return (1);
		pid = fork();
		if (pid == -1)
		{
			if (cmds[i + 1])
			{
				close(fd[0]);
				close(fd[1]);
			}
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					exit(1);
				close(prev_fd);
			}
			if (cmds[i + 1])
			{
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) == -1)
					exit(1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		i++;
	}
	while(wait(&status) > 0)
		;
	return (0);
}

int main(void)
{
    char *cmd1[] = {"/bin/echo", "hola mundo", NULL};
    char *cmd2[] = {"/bin/sed", "s/o/0/g", NULL};
    char *cmd3[] = {"/usr/bin/grep", "0", NULL};
    char **pipeline1[] = {cmd1, cmd2, cmd3, NULL};

    char *cmd4[] = {"/bin/ls", NULL};
    char *cmd5[] = {"/usr/bin/grep", "picoshell", NULL};
    char **pipeline2[] = {cmd4, cmd5, NULL};

    printf("Test 1: echo → sed → grep\n");
    if (picoshell(pipeline1) == 0)
        printf("✅ Success\n\n");
    else
        printf("❌ Error\n\n");

    printf("Test 2: ls → grep\n");
    if (picoshell(pipeline2) == 0)
        printf("✅ Success\n");
    else
        printf("❌ Error\n");

    return 0;
}
