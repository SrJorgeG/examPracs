#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include<sys/select.h>
#include<arpa/inet.h>
#include<netinet/ip.h>

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = -1;
	while ((*buf)[++i])
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

int count = 0, max_fd = 0, ids[65536];
char *msgs[65536], buf_read[1001], buf_write[42];
fd_set read_fds, write_fds, all_fds;

void fatal_error()
{
    write(2, "Fatal error\n", 12);
    exit(1);
}

void notify_other(int author, char *str)
{
	for (int fd = 0; fd <= max_fd; fd++)
		if (FD_ISSET(fd, &write_fds) && fd != author)
			send(fd, str, strlen(str), 0);
}

void send_msg(int fd)
{
	char *msg;
    while(extract_message(&(msgs[fd]), &msg))
    {
		sprintf(buf_write, "client %d: ", ids[fd]);
        notify_other(fd, buf_write);
        notify_other(fd, msg);
        free(msg);
    }
}

int main(int ac, char *av[])
{
	if (ac != 2)
		write(2, "Wrong number of arguments\n", 26), exit(1);
	FD_ZERO(&all_fds);
	// Create Socket -----------------------------------------------------------------------------
	max_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (max_fd < 0)
        fatal_error();
    FD_SET(max_fd, &all_fds);
	int sockfd = max_fd;
	// Inizialiting and Binding Socket, Socket listening -----------------------------------------
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(av[1]));
	if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) || listen(sockfd, 128))
		fatal_error();
	// Infinite loop  ----------------------------------------------------------------------------
	while (1)
	{
		// read, write = all; Select -------------------------------------------------------------
		read_fds = write_fds = all_fds;
		if (select(max_fd + 1, &read_fds, &write_fds, NULL, NULL) < 0)
			fatal_error();
		for (int fd = 0; fd <= max_fd; fd++)
			if (FD_ISSET(fd, &read_fds))
				if (fd == sockfd)
				{
					// Accepting -----------------------------------------------------------------
					socklen_t addr_len = sizeof(servaddr);
					int client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &addr_len);
					if (client_fd >= 0)
					{
						// Register new client if accept succeeds --------------------------------
						if(client_fd > max_fd)
							max_fd = client_fd;
						ids[client_fd] = count++;
						msgs[client_fd] = NULL;
						FD_SET(client_fd, &all_fds);
						sprintf(buf_write, "server: client %d just arrived\n", ids[client_fd]);
						notify_other(client_fd, buf_write);
						break;
					}
				}
				else
				{
					// Recieveing a 1000 byte char* from fd into buf_read ------------------------
					int bytes_read = recv(fd, buf_read, 1000, 0);
					if (bytes_read <= 0)
					{
						// Remove client if recieve fails ----------------------------------------
						sprintf(buf_write, "server: client %d just left\n", ids[fd]);
						notify_other(fd, buf_write);
						free(msgs[fd]);
						FD_CLR(fd, &all_fds);
						close(fd);
						break;
					}
					buf_read[bytes_read] = '\0';
					msgs[fd] = str_join(msgs[fd], buf_read);
					send_msg(fd);
				}
	}
	return 0;
}
