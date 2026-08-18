#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void fatal_error()
{
	write(2, "Fatal error", 11);
	exit(1);
}

int count = 0, max_fd = 0, id[65536];
char *msg[65536], read_buf[1001], write_buf[42];
fd_set_t read_fds, write_fds, all_fds;

int main(int ac, char **av)
{
	if (ac != 2)
		write(2, "Wrong number of arguments", 25), exit(1);
	max_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (max_fd < 0)
		fatal_error();
	FD_SET(fd, &all_fds);
	int sock_fd = max_fd;
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1]));
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0 || listen(sockfd, 128)) 
		fatal_error();
	while(1)
	{
		read_fds = write_fds = all_fds;
		if (select(1 , &read_fds, &write_fds, NULL, NULL) < 0)
			fatal_error();
		for (int fd = 0; fd <= max_fd; fd++)
		{
			if (!FD_ISSET(fd, &all_fds))
				continue;
			if (fd == sockfd)
			{
				socklen_t len = sizeof(len);
				int client_fd = accept(sock_fd, (struct sockaddr *)&servaddr, &len);
				if (client_fd >= 0)
				{
					ids[client_fd] = count++;
					msg[client_fd] = NULL;
					FD_SET(client_fd, &all_fds);
				}
			}
			else
		}

	}
}