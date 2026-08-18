#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <netinet/in.h>

int count = 0, maxfd = 0, ids[65536];
char *msgs[65536], rbuf[65536], wbuf[64];
fd_set rfds, wfds, afds;

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

void fatal(void)
{
	write(2, "Fatal error\n", 12);
	exit(1);
}

void notify(int author, char *s)
{
	for (int fd = 0; fd <= maxfd; fd++)
		if (FD_ISSET(fd, &wfds) && fd != author)
			send(fd, s, strlen(s), 0);
}

int main(int ac, char **av)
{
	if (ac != 2) { write(2, "Wrong number of arguments\n", 26); exit(1); }

	FD_ZERO(&afds);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) fatal();
	maxfd = sockfd;
	FD_SET(sockfd, &afds);

	struct sockaddr_in sa;
	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(2130706433);
	sa.sin_port = htons(atoi(av[1]));
	if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa))) fatal();
	if (listen(sockfd, 128)) fatal();

	while (1)
	{
		rfds = wfds = afds;
		if (select(maxfd + 1, &rfds, &wfds, NULL, NULL) < 0) continue;
		for (int fd = 0; fd <= maxfd; fd++)
		{
			if (!FD_ISSET(fd, &rfds)) continue;
			if (fd == sockfd)
			{
				socklen_t len = sizeof(sa);
				int cfd = accept(sockfd, (struct sockaddr *)&sa, &len);
				if (cfd < 0) break;
				if (cfd > maxfd) maxfd = cfd;
				ids[cfd] = count++;
				msgs[cfd] = NULL;
				FD_SET(cfd, &afds);
				sprintf(wbuf, "server: client %d just arrived\n", ids[cfd]);
				notify(cfd, wbuf);
				break;
			}
			else
			{
				int r = recv(fd, rbuf, sizeof(rbuf) - 1, 0);
				if (r <= 0)
				{
					sprintf(wbuf, "server: client %d just left\n", ids[fd]);
					notify(fd, wbuf);
					free(msgs[fd]);
					msgs[fd] = NULL;
					FD_CLR(fd, &afds);
					close(fd);
					break;
				}
				rbuf[r] = 0;
				msgs[fd] = str_join(msgs[fd], rbuf);
				char *msg;
				while (extract_message(&msgs[fd], &msg))
				{
					sprintf(wbuf, "client %d: ", ids[fd]);
					notify(fd, wbuf);
					notify(fd, msg);
					free(msg);
				}
			}
		}
	}
}
