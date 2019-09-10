#include <stdio.h>
#include <stdlib.h>
#include "winsocket.h"
/*
 * Accepts an incomming Message on an listen-socket and passes it to a worker-Socket
 */
void acceptSocket(SOCKET* listener, SOCKET* worker) {
	char bufIP4[INET_ADDRSTRLEN];
	SOCKADDR_IN clientAdress;
	memset(&clientAdress, 0, sizeof(clientAdress));
	int workerlength = sizeof(worker);
	*worker = accept(*listener, &clientAdress, &workerlength);
	if (*worker == INVALID_SOCKET) {
		printf("Error, accept() failed.\n");
		exit(EXIT_FAILURE);
	}
	printf(" New Client connection socket fd is [%d], Ip : [%s], port : [%d] \n", *worker, inet_ntop(AF_INET, &clientAdress.sin_addr, bufIP4, sizeof(bufIP4)), ntohs(clientAdress.sin_port));
}

/*
 * This function creates a Socket-instance. Therefore it needs a familytype.
 * Also what type of Data you want to pass, for e.g. UDP or TCP and the used
 * handshake protocol.
 */
SOCKET createSocket(int family, int type, int protocol) {
	SOCKET sock;
	initWinSocket();
	sock = socket(family, type, protocol);
	if (sock == SOCKET_ERROR) {
		printf("Error, couldn't create Socket from type [%d]: [%i]\n", type, WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Socket successfully created.\n");
	return sock;
}

/*
 * This function binds an given socket to a given address and its port
 */
void bindSocket(SOCKET* socket, unsigned long adress, int port) {
	SOCKADDR_IN serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(adress);
	serverAddress.sin_port = htons(port);
	if (bind(*socket, &serverAddress, sizeof(serverAddress)) == SOCKET_ERROR){
		printf("Error, couldn't bind Socket : [%i]\n", WSAGetLastError());
	}
	printf("Socket successfully bound.\n");
}

/*
 * This funktion sets a given socket into listen-mode.
 */
void listenSocket(SOCKET* socket) {
	if(listen(*socket, 0) == SOCKET_ERROR) {
		printf("Error, couldn't set Socket in listen mode : [%i]\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Socket is now in listen mode.\n");
}

/*
 * Helpingmethod for concating two strings
 */
void concatString(char* source, char* add)
{
	while (*source)
		source++;

	while (*add)
	{
		*source = *add;
		add++;
		source++;
	}
	*source = '\0';
}

/*
 * This function initializes the Windows-socket
 */
int initWinSocket() {
    WSADATA wsa;
	int returnCode = WSAStartup(*((int*)"2,2"), &wsa);
	if (returnCode != 0) {
		printf("Error while try to init WinSocket : [%i]\n", returnCode);
		exit(EXIT_FAILURE);
	}
	printf("Windows Socket initialized.\n");
}
