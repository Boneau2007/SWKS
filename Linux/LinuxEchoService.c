#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "linuxsocket.h"
#include "LinuxEchoService.h"

const char* stdWelcomeMessage = "Welcome to Echo-Server v1.0\n\r";
char* stdReturnMessage = "Server answered: ";
const int bufferSize = 256;
int port;
int start(int iPort) {
	int tcpListener;
	port = iPort;
	int workers[MAX_WORKER];
	char* buff = (char*)calloc(bufferSize, sizeof(char));
	char* message = (char*)calloc(sizeof(stdReturnMessage) + sizeof(buff), sizeof(char));
	tcpListener = createSocket(AF_INET, SOCK_STREAM, 0);
	bindSocket(&tcpListener, INADDR_ANY, port);
	listenSocket(&tcpListener);
	struct sockaddr_in serverAddress;

	fd_set fdset;
	FD_ZERO(&fdset);
	int returnCode = 0;

	for (int i = 0; i < MAX_WORKER; i++) {
		workers[i] = -1;
	}

	while (1) {
		//Initializing and Reinitializing of the FS_Set because of modifing through System Calls
		FD_ZERO(&fdset);
		FD_SET(tcpListener, &fdset);
		for (int i = 0; i < MAX_WORKER; i++) {
			if (workers[i] != -1) {
				FD_SET(workers[i], &fdset);
			}
		}

		if (select(FD_SETSIZE, &fdset, NULL, NULL, NULL) == -1) {
			printf("Error in select\n");
			return EXIT_FAILURE;
		}
		//If something wants to connect
		if (FD_ISSET(tcpListener, &fdset)) {
			for (int i = 0; i < MAX_WORKER; i++) {
				if (workers[i] == -1) {
					acceptSocket(&tcpListener, &workers[i]);
					if (send(workers[i], stdWelcomeMessage, 33, 0) == -1){
						printf("Error, while try to send.\n");
					}
					break;
				}
			}
		}
		//Else something happens with the I\O of the Socket descriptors
		else {
			for (int i = 0; i <= MAX_WORKER; i++) {
				if (FD_ISSET(workers[i], &fdset)) {
					if (buff) {
						returnCode = recv(workers[0], buff, bufferSize, 0);
					}
					if (returnCode == 0) {
						closeSocket(&workers[i]);
						printf("Connection with Client [%d] has been closed\n", i);
					}
					else {
						if (returnCode < bufferSize - 1) {
							buff[returnCode + 1] = '\0';
							if (message) {
								concatString(message, stdReturnMessage);
								concatString(message, buff);
								send(workers[i], message, strlen(message), 0);
								memset(message, '\0', sizeof(message));
							}
						}
					}
				}
			}
		}
	}
	free(message);
	free(buff);
	exit(EXIT_SUCCESS);
}

int pause() {
}

int resume() {
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
