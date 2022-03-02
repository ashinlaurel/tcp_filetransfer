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
#define IP "127.0.0.1"
#define SA struct sockaddr

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
	char data[15] = {0};
	int c;

	// for (int i = 0; i < 10 && c != EOF; i++)
	// {
	// 	c = fgetc(fp);
	// 	data[i] = c;
	// }
	char str[100];
	if (fgets(str, 11, fp) != NULL)
	{
		printf("%s", str);
		strcat(data, str);
	}

	if (!write(sockfd, data, strlen(data)))
	{
		perror("[-]Error in sending file.");
		exit(1);
	}
	bzero(data, 10);
}

// Driver function
int main()
{

	int sockfd, connfd;
	socklen_t len = 0;
	struct sockaddr_in servaddr, cli;
	int n = 0;

	char filename[50] = "";
	char buff[MAX];
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

	// taking input for port number form user
	bzero(buff, sizeof(buff));
	printf("\nEnter the server port number:");

	while ((buff[n++] = getchar()) != '\n')
		;
	strcpy(port_number, buff);

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(atoi(port_number));

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("Socket bind failed...\n");
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

	bzero(buff, MAX);

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
		printf("Sending Empty file '%s'\n", path);
	}

	// After chatting close the socket
	close(sockfd);
	exit(1);
}
