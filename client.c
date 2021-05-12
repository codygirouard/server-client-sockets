/*
	Cody Girouard
*/
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 256 
void chat(int sock) 
{ 
	char buff[MAX]; 
	int n; 

	// infinite loop for communication between client
	while (1) { 
		bzero(buff, sizeof(buff)); 
		printf("input: "); 
		n = 0; 

		// read input from user
		while ((buff[n++] = getchar()) != '\n'); 

		// send input to client
		write(sock, buff, sizeof(buff)); 

		// clear buffer
		bzero(buff, sizeof(buff)); 

		// read message from server into buffer
		read(sock, buff, sizeof(buff)); 
		printf("output: %s", buff); 
		if ((strncmp(buff, "quit", 4)) == 0) { 
			printf("goodbye!\n"); 
			break; 
		} 
	} 
} 

int main(int argc, char **argv) 
{ 
	if (argc != 2) // incorrect command line format
	{
		printf("usage : ./client <port_number>\n");
		exit(1);
	}
	// declare socket
	int sock, connfd; 
	struct sockaddr_in server, cli; 

	// create socket 
	sock = socket(AF_INET, SOCK_STREAM, 0); 

	// verify socket successfully created
	if (sock == -1) { 
		printf("failed to create socket...\n"); 
		exit(0); 
	}
	bzero(&server, sizeof(server)); 

	// assign ip, host, and port 
	char host[] = "cse01";
	struct hostent *hosten;
	hosten = gethostbyname(host);
	if (hosten == NULL) // ip to correspond to hostname not found
	{
		printf("hostname not found\n");
		exit(EXIT_FAILURE);
	}
	memcpy(&server.sin_addr, hosten->h_addr_list[0], hosten->h_length); // insert ip into sockaddr_in's sin_addr
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	inet_pton(AF_INET, host, &server.sin_addr);

	// connect the client socket to server socket 
	if (connect(sock, (struct sockaddr*)&server, sizeof(server)) != 0) { 
		printf("connection with server failed...\n"); 
		exit(0); 
	} 
	
	// function for chat 
	chat(sock); 

	// close the socket 
	close(sock); 
} 
