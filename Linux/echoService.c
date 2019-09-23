#include "echoService.h"

char* stdReturnMessage = "Server answered: ";


/*
 * @function	initEchoService
 * @abstract	Initializes the echo-service
 * @discuss 	This function accepts a communication initiation
 * 				and prepares the sockets.
 * @param		listener	Pointer-Id of the socket which passes the communication
 * @param		worker		Pointer-Id of the socket which has to accept the communication
 * @param		message		Message you want to send
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int initEchoService(int* listener, int* worker, const char* message){
	if(!acceptSocket(listener, worker)){
		if (write(*worker, message, strlen(message)) == -1){
			fprintf(stderr,"\nError, while try to send.\n");
			return EXIT_FAILURE;
		}else{
			fprintf(stdout,"\nWelcome message has been send.\n");
			return EXIT_SUCCESS;
		}
	}else{
		return EXIT_FAILURE;
	}
}

/*
 * @function	handleEchoService
 * @abstract	Handles communication
 * @discuss 	This function handles the tcp communication of the echo service.
 * @param		socket		Pointer-Id of the socket
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int handleEchoService(int* socket){
	char* buff = (char*)calloc(BUFF_SIZE, sizeof(char));
	char* message = (char*)calloc(BUFF_SIZE, sizeof(char));
	int messageLength = 0;
	if((messageLength = recv(*socket, buff, BUFF_SIZE, 0)) == -1){
		switch (errno){
		case EWOULDBLOCK: fprintf(stderr,"\nOperation would block. Try again\n");
						  return EWOULDBLOCK;
		default: fprintf(stderr,"\nError, recv() failed.\n");
				 return -1;
		}
	}else{
		fprintf(stdout,"Worker[%d] recieved [%d] bytes \n",  *socket, messageLength);
		if(strncmp(buff, "CLOSE",5)==0){
			fprintf(stdout,"Worker[%d] recieved a CLOSE call. \n", *socket);
			messageLength = 0;
		}
		if (messageLength == 0) {
			free(buff);
			free(message);
			return EXIT_FAILURE;
		}else {
			if (messageLength < BUFF_SIZE - 1) {
				strncpy(message, stdReturnMessage, strlen(stdReturnMessage));
				strncat(message, buff, strlen(buff));
				fprintf(stdout,"Send Message at worker : [%d] \n", *socket);
				write(*socket, message, strlen(message));
				fprintf(stdout,"Message has been send\n");
			}else{
				char* errorMsg = "Message was to large to send";
				write(*socket, errorMsg, strlen(errorMsg));
				free(buff);
				free(message);
				return EXIT_FAILURE;
			}
		}
	}
	free(buff);
	free(message);
	return EXIT_SUCCESS;
}


/*
 * @function	closeConnection
 * @abstract	Closes communication to client
 * @discuss 	This function handles the tcp close call, to shutdown a
 * 				communication with a client and frees the Socket.
 * @param		socket		Pointer-Id of the socket
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int closeConnection(int* socket){
	if(shutdown(*socket,SHUT_RDWR) < 0){
		fprintf(stderr,"\nError on shutdown connection\n");
		return EXIT_FAILURE;
	}
	*socket = -1;
	fprintf(stdout,"\nConnection has been shutdown\n");
	return EXIT_SUCCESS;
}
