#include "Server.h"
#include "Sockets.h"


const int TCPPORT = 9001;
const int UDPPORT = 9002;
const char* stdWelcomeMessage = "Welcome to Server v1.0\n\r";
const char* stdReturnMessage = "Server answered: ";

void start() {

	SOCKET workers[20];
	char* buff = (char*)calloc(bufferSize, sizeof(char));
	char* message = (char*)calloc(sizeof(stdReturnMessage) + sizeof(buff), sizeof(char));
	SOCKET tcpListener = NULL;
	tcpListener = createSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bindSocket(&tcpListener, INADDR_ANY, TCPPORT);
	listenSocket(&tcpListener);
	SOCKADDR_IN serverAddress;

	FD_SET fdset;
	FD_ZERO(&fdset);
	int returnCode = 0;



	for (int i = 0; i < MaxWorker; i++) {
		workers[i] = INVALID_SOCKET;
	}

	while (1) {
		//Initializing and Reinitializing of the FS_Set because of modifing through System Calls
		FD_ZERO(&fdset);
		FD_SET(tcpListener, &fdset);
		for (int i = 0; i < MaxWorker; i++) {
			if (workers[i] != INVALID_SOCKET) {
				FD_SET(workers[i], &fdset);
			}
		}

		if (select(FD_SETSIZE, &fdset, NULL, NULL, NULL) == SOCKET_ERROR) {
			printf("Error in select\n");
			return EXIT_FAILURE;
		}
		//If something wants to connect
		if (FD_ISSET(tcpListener, &fdset)) {
			for (int i = 0; i < MaxWorker; i++) {
				if (workers[i] == INVALID_SOCKET) {
					acceptSocket(&tcpListener, &workers[i]);
					if (send(workers[i], stdWelcomeMessage, strlen(stdWelcomeMessage) + 1, 0) == SOCKET_ERROR)
						printf("Error, while try to send.\n");
					break;
				}
			}
		}
		//Else something happens with the I\O of the Socket descriptors
		else {
			for (int i = 0; i <= MaxWorker; i++) {
				if (FD_ISSET(workers[i], &fdset)) {
					if (buff) {
						returnCode = recv(workers[0], buff, bufferSize, 0);
					}
					if (returnCode == 0) {
#ifdef __unix__
						close(worker[i]);
#elif __MSDOS__ || __WIN32__ || _MSC_VER
						closesocket(workers[i]);
#endif
						workers[i] = INVALID_SOCKET;
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
	WSACleanup();
	exit(EXIT_SUCCESS);
}

void pause() {
}

void stop() {
}