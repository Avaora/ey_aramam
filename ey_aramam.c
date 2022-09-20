#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char	shut_flag;
char	rx_buf[258];
char	tx_buf[258];
void	*ft_read(void *);
void	*ft_write(void *);

int main(int argc, char *argv[])
{
	pthread_t	read_td;
	pthread_t	write_td;
	int	clientsd;
	int	serversd;
	int	listensd;
	struct	sockaddr_in client_sock;
	struct	sockaddr_in server_sock;

	if (argc == 2)
	{
		shut_flag = 1;
		client_sock.sin_family = AF_INET;
		client_sock.sin_port = htons(4096);
		client_sock.sin_port = inet_addr(argv[1]);

		printf("You are client now\n");
		clientsd = socket(AF_INET, SOCK_STREAM, 0);
		if (clientsd < 0)
		{
			printf("ERROR: Socket could not created");
			return (0);
		}
		else if (connect(clientsd, 
			(struct sockaddr*)&client_sock, sizeof(client_sock)) == 0)
		{
			printf("Connected to server");

			pthread_create(&read_td, NULL, ft_read, &clientsd);
			pthread_create(&write_td, NULL, ft_write, &clientsd);

			pthread_join(read_td, NULL);
			pthread_join(write_td, NULL);

			close(clientsd);
		}
		else
			printf("ERROR: Could not connected to server");
	}

	else if (argc == 1)
	{
		shut_flag = 1;
		server_sock.sin_family = AF_INET;
		server_sock.sin_port = htons(4096);
		server_sock.sin_addr.s_addr = INADDR_ANY;

		printf("You are server now\n");
		listensd = socket(AF_INET, SOCK_STREAM, 0);

		if (listensd < 0)
		{
			printf("ERROR: Socket could not created\n");
			return (0);
		}
		else if (bind(listensd, 
			(struct sockaddr*)&server_sock, sizeof(server_sock)) == 0)
		{
			printf("Binding successful\n");
			listen(listensd, 5);
			printf("Waiting for a connection request...\n");
			serversd = accept(listensd, 
			(struct sockaddr*)&client_sock, (socklen_t *)sizeof(client_sock));
			if (serversd >= 0)
			{
				printf("A client connected with ip address: %s\n", 
						ntohs(client_sock.sin_addr.s_addr));
				pthread_create(&read_td, NULL, ft_read, &serversd);
				pthread_create(&write_td, NULL, ft_write, &serversd);

				pthread_join(read_td, NULL);
				pthread_join(write_td, NULL);

				close(serversd);
			}
			else
				printf("ERROR: Connection request could not accepted\n");
		}
		else
			printf("ERROR: Socket could not bind\n");
		close(listensd);
	}
	else
		printf("Invalid Option\n");
	return (0);
}

void *ft_read(void *ptr)
{
	int	sock_desc;
	sock_desc = *((int *) ptr);
	while (1)
	{
		read(sock_desc, rx_buf, sizeof(rx_buf));
		if ((rx_buf[0] == 0x24) || (shut_flag == 0))
		{
			printf("Connection closed by remote\n");
			shut_flag = 0;
			break ;
		}
		write(1, rx_buf, strlen(rx_buf));
	}
	return (0);
}

void *ft_write(void *ptr)
{
	int	sock_desc;
	sock_desc = *((int *) ptr);
	while (1)
	{
		fgets(tx_buf, sizeof(tx_buf), stdin);
		if ((tx_buf[0] == 0x24) || (shut_flag == 0))
		{
			printf("Connection ended\n");
			write(sock_desc, tx_buf, sizeof(tx_buf));
			shut_flag = 0;
			break ;
		}
		write(sock_desc, tx_buf, sizeof(tx_buf));
	}
	return (0);
}
