#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

// definitions
#define MAX 1024
#define PORT 4444
#define IP "127.0.0.1"
#define SA struct sockaddr

// Function designed for handling messages between client and server.
// void chatter(int connfd)
// {
// 	char buff[MAX];
// 	int n;

// 	while (1)
// 	{
// 		bzero(buff, MAX);

// 		// read the message from client and copy it in buffer. Then print it.
// 		read(connfd, buff, sizeof(buff));
// 		printf("From client: %s", buff);

// 		if (strncmp(".", buff, 1) == 0)
// 		{
// 			break;
// 		}
// 	}
// }

// function to check whether file exists
int DoesFileExists(const char *path)
{
	// Try to open file
	FILE *fptr = fopen(path, "r");

	// If file does not exists
	if (fptr == NULL)
		return 0;

	// File exists hence close file and return true.
	fclose(fptr);

	return 1;
}

// function to send file
void send_file(FILE *fp, int sockfd)
{
	int n;
	char data[10] = {0};
	int c;

	for (int i = 0; i < 10 && c != EOF; i++)
	{
		c = fgetc(fp);
		data[i] = c;
	}

	if (send(sockfd, data, sizeof(data), 0) == -1)
	{
		perror("[-]Error in sending file.");
		exit(1);
	}
	bzero(data, 10);
}

// Driver function
int main()
{
	char filename[50] = "";
	int sockfd, connfd;
	socklen_t len = 0;
	struct sockaddr_in servaddr, cli;

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

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA *)&cli, &len);
	if (connfd < 0)
	{
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	char buff[MAX];
	int n;
	bzero(buff, MAX);
	// printf("From Server: Hello client. What is your name?\n");
	// strcpy(buff, "Hello. What is your name?");
	// write(connfd, buff, sizeof(buff));

	// read the filename sent by the server
	read(connfd, buff, sizeof(buff));
	// printing file name
	printf("Client has requested the file %s \n", buff);
	strcpy(filename, buff);

	char path[100] = "./";
	strcat(path, filename);

	// Check if file exists or not
	if (DoesFileExists(path))
	{
		printf("File exists at path '%s'\n", path);
		FILE *fptr = fopen(path, "r");
		send_file(fptr, connfd);
		fclose(fptr);
	}
	else
	{
		printf("File does not exists at path '%s'\n", path);
	}

	// After chatting close the socket
	close(sockfd);
	exit(1);
}
