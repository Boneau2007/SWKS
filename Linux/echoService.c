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
	acceptSocket(listener, worker);
	if (write(*worker, message, strlen(message)) == -1){
		printf("\nError, while try to send.\n");
		return EXIT_FAILURE;
	}else{
		printf("\nWelcome message has been send.\n");
		return EXIT_SUCCESS;
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
	if (buff) {
		messageLength = recv(*socket, buff, BUFF_SIZE, 0);
		printf("Worker[%d] recieved [%d] bytes \n",  *socket, messageLength);
		if(strncmp(buff, "CLOSE",5)==0){
			printf("Worker[%d] recieved a CLOSE call. \n", *socket);
			messageLength = 0;
		}
	}
	if (messageLength == 0) {
		free(buff);
		free(message);
		return EXIT_FAILURE;
	}else {
		if (messageLength < BUFF_SIZE - 1) {
			if (buff) {
				strncpy(message, stdReturnMessage, strlen(stdReturnMessage));
				strncat(message, buff, strlen(buff));
				printf("Send Message at worker : [%d] \n", *socket);
				write(*socket, message, strlen(message));
				printf("Message has been send\n");
			}
		}else{
			char* errorMsg = "Message was to large to send";
			write(*socket, errorMsg, strlen(errorMsg));
			return EXIT_FAILURE;
		}
	}
	free(buff);
	free(message);
	return EXIT_SUCCESS;
}
