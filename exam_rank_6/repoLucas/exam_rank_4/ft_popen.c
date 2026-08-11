#include <unistd.h>
#include <stdlib.h>

// Nombre de la tarea: ft_popen
// Archivos esperados: ft_popen.c
// Funciones permitidas: pipe, fork, dup2, execvp, close, exit
// ----------------------------------------------------------- ---------------------------

// Escriba la siguiente función:

// int ft_popen(const char *file, char *const argv[], char type);

// La función debe ejecutar el archivo ejecutable con los argumentos argv (utilizando execvp).
// Si type es «r», la función debe devolver un descriptor de archivo conectado a la salida del comando.
// Si type es «w», la función debe devolver un descriptor de archivo conectado a la entrada del comando.
// En caso de error o parámetro no válido, la función debe devolver -1.

// Por ejemplo, la función se podría utilizar así:

// int main()
// {
//     int  fd;
//     char *line;

//     fd = ft_popen(«ls», (char *const []){«ls», NULL}, “r”);
//     while ((line = get_next_line(fd)))
//         ft_putstr(line);
//     return (0);
// }


// int    main() {
//     int    fd = ft_popen(«ls», (char *const []){«ls», NULL}, “r”);
//     dup2(fd, 0);
//     fd = ft_popen(«grep», (char *const []){“grep”, «c», NULL}, “r”);
// 	char    *línea;
//     while ((línea = get_next_line(fd)))
//         printf(«%s», línea);
// }


// Pistas:
// ¡No dejes escapar los descriptores de archivos!
// Este ejercicio está inspirado en popen() de libc.
int ft_popen(const char *file, char *const argv[], char type)
{
	int pid;
	int fd[2];

	if (!file || !argv || (type != 'r' && type != 'w'))
		return -1;
	if (pipe(fd) == -1)
		return -1;
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return -1;
	}
	if (pid == 0)
	{
		if (type == 'r')
			dup2(fd[1], STDOUT_FILENO);
		if (type == 'w')
			dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(-1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return fd[0];
	}
	if (type == 'w')
	{
		close(fd[0]);
		return fd[1];
	}
	return -1;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ft_popen2(const char *file, char *const argv[], char type)
{
	int	fd[2];
	int	pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
			dup2(fd[1], STDOUT_FILENO);
		if (type == 'w')
			dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(-1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	if (type == 'w')
	{
		close(fd[0]);
		return (fd[1]);
	}
	return (-1);
}

int main(void)
{
    int fd;
    char buffer[1024];
    ssize_t n;

    printf("=== Test 1: lectura (ls) ===\n");
    fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
    if (fd == -1)
    {
        perror("ft_popen");
        return 1;
    }
    while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[n] = '\0';
        write(STDOUT_FILENO, buffer, n);
    }
    close(fd);

    printf("\n=== Test 2: escritura (cat) ===\n");
    fd = ft_popen("cat", (char *const[]){"cat", NULL}, 'w');
    if (fd == -1)
    {
        perror("ft_popen");
        return 1;
    }
    write(fd, "Hola mundo desde ft_popen!\n\0", 28);
    close(fd);

    printf("\n=== Test 3: comando inválido ===\n");
    fd = ft_popen("noexiste", (char *const[]){"noexiste", NULL}, 'r');
    if (fd == -1)
        printf("Correcto: devuelve -1 en error\n");

    return 0;
}

