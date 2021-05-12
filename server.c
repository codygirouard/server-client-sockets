/*
	Cody Girouard
*/

#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#define MAX_BUFF 256 

void chat(int sock) 
{ 
	char buff[MAX_BUFF]; 
	int n; 
	// infinite loop for communication 
	while (1) { 
		bzero(buff, MAX_BUFF); 

		// wait for and read message from client and put it in buffer 
		read(sock, buff, sizeof(buff));
		for (int i = 0; i < MAX_BUFF; i++)
		{
			if (buff[i] == '\n')
			{
				buff[i] = '\0';
				break;
			}
		}
		const char *str = buff;
		struct addrinfo hints, *infoptr;
		hints.ai_family = AF_INET;
		int result = getaddrinfo(str, NULL, &hints, &infoptr);
		if (result) {
			// failed to connect
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
			exit(1);
		}
		struct addrinfo *p;
		char host[256];

		for (p = infoptr; p != NULL; p = p->ai_next) {

			getnameinfo(p->ai_addr, p->ai_addrlen, host, sizeof (host), NULL, 0, NI_NUMERICHOST);

		}


		char req[10000]; 
		struct sockaddr_in serveraddr; 
		int port = 80;

		int reqSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (reqSocket < 0)
			printf("failed to create socket...\n");
		else
			printf("success: create socket...\n");
		
		bzero((char *) &serveraddr, sizeof(serveraddr));
 		serveraddr.sin_family = AF_INET;
 
		bcopy((char *)infoptr->ai_addr, (char *)&serveraddr.sin_addr.s_addr, infoptr->ai_addrlen);

		serveraddr.sin_port = htons(port);
     
		if (connect(reqSocket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
			printf("failed to connect...\n");
		else
			printf("connection success...\n");

		bzero(req, 10000);
		sprintf(req, "Get / HTTP/1.1\r\nHost: %s\r\n\r\n\r\n", host);
     
		printf("\n%s", req);

		if (send(reqSocket, req, strlen(req), 0) < 0)
			printf("failed to send...\n");

		bzero(req, 10000);
     
		recv(reqSocket, req, 9999, 0);
		printf("\n%s", req);
		printf("\nhello");
		freeaddrinfo(infoptr);
		close(reqSocket);

		// and send that buffer to client 
		// write(sock, buff, sizeof(buff)); 

		// if msg contains "quit" then server and client chat ends 
		if (strncmp("quit", buff, 4) == 0) { 
			printf("goodbye!\n"); 
			break; 
		} 
	} 
} 

// main function
int main(int argc, char **argv) 
{ 
	// declare socket and connection 
	int sock, conn, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sock = socket(AF_INET, SOCK_STREAM, 0); 
	if (sock < 0) { 
		printf("failed to create socket...\n"); 
		exit(0); 
	} 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(atoi(argv[1]));


	// bind new socket to ip -> verify 
	if ((bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("failed to bind socket...\n"); 
		exit(0); 
	} 

	// set up server to listen
	if ((listen(sock, 5)) != 0) { 
		printf("failed to listen...\n"); 
		exit(0); 
	} 
	else
		printf("waiting for client...\n"); 
	len = sizeof(cli); 

	// accept data from client 
	conn = accept(sock, (struct sockaddr*)&cli, &len); 
	if (conn < 0) { 
		printf("failed to accept client...\n"); 
		exit(0); 
	} 
	else
		printf("accepted client...\n"); 

	// chat function for server/client 
	chat(conn); 

	// close socket
	close(sock); 
} 
