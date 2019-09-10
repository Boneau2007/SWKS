#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "linuxsocket.h"
/*
 * Accepts an incomming Message on an listen-socket and passes it to a worker-Socket
 */
void acceptSocket(int* listener, int* worker) {
	struct sockaddr_in clientAddress;
	memset(&clientAddress, 0, sizeof(clientAddress));
	int workerlength = sizeof(worker);
	*worker = accept(*listener, (struct sockaddr_in *)&clientAddress, &workerlength);
	if (*worker == -1) {
		printf("Error, accept() failed.\n");
		exit(EXIT_FAILURE);
	}
	printf(" New Client connection to socket, fd is [%d], Ip : [%s], port : [] \n", *worker, inet_ntoa(clientAddress.sin_addr));
}

/*
 * This function creates a Socket-instance. Therefore it needs a familytype.
 * Also what type of Data you want to pass, for e.g. UDP or TCP and the used
 * handshake protocol.
 */
int createSocket(int family, int type, int protocol) {
	int sock;
	sock = socket(family, type, protocol);
	if (sock == -1) {
		printf("Error, couldn't create Socket from type [%d]: [%i]\n", type, sock);
		exit(EXIT_FAILURE);
	}
	printf("Socket successfully created.\n");
	return sock;
}

/*
 * This function binds an given socket to a given address and its port
 */
void bindSocket(int* socket, unsigned int address, int port) {
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(address);
	serverAddress.sin_port = htons(port);
	if (bind(*socket, (struct sockaddr_in *)&serverAddress, sizeof(serverAddress)) == -1){
		printf("Error, couldn't bind Socket :\n");
	}
	printf("Socket successfully bound.\n");
}

/*
 * This funktion sets a given socket into listen-mode.
 */
void listenSocket(int* socket) {
	if(listen(*socket, 0) == -1) {
		printf("Error, couldn't set Socket in listen mode :\n");
		exit(EXIT_FAILURE);
	}
	printf("Socket is now in listen mode.\n");
}
/*
 * This funktion closes a socket
 */
void closeSocket(int* socket) {
	if(close(*socket) == -1) {
		printf("Error, couldn't close Socket.\n");
		exit(EXIT_FAILURE);
	}
	printf("Socket is now closed.\n");
}
