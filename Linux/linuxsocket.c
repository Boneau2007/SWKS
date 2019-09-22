#include "linuxsocket.h"

/*
 * @function	acceptSocket
 * @abstract	Accepts a client connection
 * @discuss 	This function accepts the client connection and passes it from the
 * 				listing socket to another socket.
 * @param		listener	Pointer-Id of the listing-socket
 * @param		worker		Pointer-Id of the working-socket
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int acceptSocket(int* listener, int* worker) {
	struct sockaddr_in clientAddress;
	memset(&clientAddress, 0, sizeof(clientAddress));
	int workerlength = sizeof(worker);
	if ((*worker = accept(*listener, (struct sockaddr *)&clientAddress, (socklen_t *)&workerlength)) == -1) {
		switch (errno){
		case EWOULDBLOCK: fprintf(stderr,"\nOperation desn't block. Try again\n");
						  return EWOULDBLOCK;
		default: fprintf(stderr,"\nError, accept() failed.\n");
				 return -1;
		}
	}else{
		fprintf(stdout,"\nNew Client connection to socket, fd is [%d], Ip : [%s], port : [%d] \n", *worker, inet_ntoa(clientAddress.sin_addr),htons(clientAddress.sin_port));
		return EXIT_SUCCESS;
	}
}

/*
 * @function	createSocket
 * @abstract	Creates a Socket with given family, type and protocol
 * @discuss 	This function creates a Socket-instance. Therefore it needs a family-type.
 * 				Also what type of data you want to pass, for e.g. UDP or TCP and the used
 * 				handshake protocol.
 * @param		family 		see sys/socket.h
 * @param		type 		see sys/socket.h
 * @param		protocol 	see sys/socket.h
 * @result		The Interger-Id of the Socket or -1 if creation failed
 */
int createSocket(int family, int type, int protocol) {
	int sock;
	sock = socket(family, type, protocol);
	if (sock == -1) {
		fprintf(stderr,"Error, couldn't create Socket from type [%d]: [%i]\n", type, sock);
		return -1;
	}
	fprintf(stdout,"Socket successfully created.\n");
	return sock;
}

/*
 * @function	bindSocket
 * @abstract	Bindes a Socket with socket, given address and port
 * @discuss 	This function binds an given socket to a given address and its port
 * @param		socket		The pointer from the Id of the socket
 * @param		address 	Address-value 
 * @param		port		Integer where the port should bound to
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int bindSocket(int* socket, unsigned int address, int port) {
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(address);
	serverAddress.sin_port = htons(port);
	if (bind(*socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1){
		fprintf(stderr,"Error, couldn't bind Socket :\n");
		return EXIT_FAILURE;
	}else{
		fprintf(stdout,"Socket successfully bound.\n");
		return EXIT_SUCCESS;
	}
}

/*
 * @function	listenSocket
 * @abstract	Sets a socket in listen mode
 * @discuss 	This function changes the behaviour of the socket and set it
 * 				to listen mode
 * @param		socket		The pointer from the Id of the socket
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int listenSocket(int* socket) {
	if(listen(*socket, 0) == -1) {
		fprintf(stderr,"Error, couldn't set Socket in listen mode :\n");
		return EXIT_FAILURE;
	}else{
		fprintf(stdout,"Socket is now in listen mode.\n");
		return EXIT_SUCCESS;
	}
}
/*
 * This funktion closes a socket
 */

/*
 * @function	closeSocket
 * @abstract	Close a socket
 * @discuss 	This function closes a socket
 * @param		socket		The pointer from the Id of the socket
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int closeSocket(int* socket) {
	if(close(*socket) == -1) {
		fprintf(stderr,"Error, couldn't close Socket.\n");
		return EXIT_FAILURE;
	}else{
		fprintf(stdout,"Socket is now closed.\n");
		return EXIT_SUCCESS;
	}
}
