#include "echoService.h"

char* stdReturnMessage = "Server answered: ";

/*
 * This function handles the tcp communication of the echo service
 */
void initEchoService(int* listenerSocket, int* workerSocket, const char* message){
	acceptSocket(listenerSocket, workerSocket);
	printf("\nSending welcome message.\n");
	if (write(*workerSocket, message, strlen(message)) == -1){
		printf("\nError, while try to send.\n");
	}else{
		printf("\nWelcome message has been send.\n");
	}
}

/*
 * This function handles the tcp communication of the echo service
 */
int handleEchoService(int* workerSocket){
	char* buff = (char*)calloc(BUFF_SIZE, sizeof(char));
	char* message = (char*)calloc(BUFF_SIZE, sizeof(char));
	int messageLength = 0;
	if (buff) {
		messageLength = recv(*workerSocket, buff, BUFF_SIZE, 0);
		printf("Worker[%d] recieved [%d] bytes \n",  *workerSocket, messageLength);
		if(strncmp(buff, "CLOSE",5)==0){
			printf("Worker[%d] recieved a CLOSE call. \n", *workerSocket);
			messageLength = 0;
		}
	}
	if (messageLength == 0) {
		free(message);
		return EXIT_FAILURE;
	}else {
		if (messageLength < BUFF_SIZE - 1) {
			if (buff) {
				strcpy(message, stdReturnMessage);
				strcat(message, buff);
				printf("Send Message at worker : [%d] \n", *workerSocket);
				write(*workerSocket, message, strlen(message));
				printf("Message has been send\n");
				memset(buff,'\0', sizeof(char)*BUFF_SIZE);
				memset(message,'\0', sizeof(char)*BUFF_SIZE);
			}
		}else{
			char* errorMsg = "Message was to large to send";
			write(*workerSocket, errorMsg, strlen(errorMsg));
			return EXIT_FAILURE;
		}
	free(message);
	return EXIT_SUCCESS;
	}
}
