#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char * message);
int main(int argc, char *argv[])
{
        int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	
	printf("argc: %d \n", argc);
        int i, str_len, len_argv = sizeof(argv);
//        for(i = 0; i < len_argv; i++){
//           printf("argv %d: %s \n", i, argv[1]);
//        }
//
	
	char message[BUF_SIZE];
	
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error.");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error.");
	
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error.");
	clnt_addr_size = sizeof(clnt_addr);
	for (i=1; i<=5;i++)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept() error.");
		else
			printf("connected client: %d \n", clnt_sock);
		while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
		{
			write(clnt_sock, message, str_len);
		}
		close(clnt_sock);
	}
	close(serv_sock);
	exit(0);
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}
