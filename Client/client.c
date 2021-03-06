#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

// definitions
#define MAX 1024
#define SA struct sockaddr

// function to receive file

void write_file(int sockfd, char filename[])
{
	int n;
	FILE *fp;
	// char *filename = "recv.txt";
	char buffer[10];
	// bzero(buffer, sizeof(buffer));

	fp = fopen(filename, "w");

	n = recv(sockfd, buffer, sizeof(buffer), 0);
	if (n <= 0)
	{
		printf("[+]Empty File %s received \n", filename);
		return;
	}
	fputs(buffer, fp);

	printf("[+]Data written in the file %s successfully.\n", filename);
	fclose(fp);
	return;
}

int main()
{

	int sockfd, connfd, new_sock;
	struct sockaddr_in servaddr, cli, new_addr;
	socklen_t addr_size;

	// imp declarations
	char buff[MAX];
	int n = 0;
	char filename[50] = "Networkingtrends.txt";
	// char filename[50] = "empty.txt";
	char IP_addr[50] = "127.0.0.1";
	char port_number[10] = "4444";

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// User Input for port and ip -------------------------------

	bzero(buff, sizeof(buff));
	printf("\nEnter server IP address:");

	n = 0;
	while ((buff[n++] = getchar()) != '\n')
		;
	strcpy(IP_addr, buff);

	bzero(buff, sizeof(buff));
	printf("\nEnter the server port number:");
	n = 0;
	while ((buff[n++] = getchar()) != '\n')
		;
	strcpy(port_number, buff);

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP_addr);
	servaddr.sin_port = htons(atoi(port_number));

	// connect the client socket to server socket
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// -- -- -- -- -- -- = user enters filename-- -- -- -- -- -- -- -- --
	bzero(buff, sizeof(buff));
	printf("\nEnter the file name:");
	scanf("%s", filename);

	bzero(buff, sizeof(buff));
	printf("[+]Requesting file %s from Server \n ", filename);
	// strcpy(buff, filename);
	write(sockfd, filename, sizeof(filename));

	write_file(sockfd, filename);

	// close the socket
	close(sockfd);
	exit(1);
}
