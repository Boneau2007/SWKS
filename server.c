#include "Server.h"
#include "Sockets.h"


#define TCP_ECHO_SERVICE_SERVICE 9001
#define NAMED_PIPE_SERVICE 9002
#define UDP_DATE_SERVICE 9003
const char* TCP_ECHO_SERVICE = "echoservice";
const char* NAMED_PIPE = "namedpipe";
const char* UDP_DATE = "game";

typedef enum {start, stop, pause, resume} MODE;
struct service{
	char* name;
	MODE mode;
}

const char* stdWelcomeMessage = "Welcome to Server v1.0\n\r";
const char* stdReturnMessage = "Server answered: ";
int initValue = -1;
if (argc != 3) {
	print("usage: main <service> <mode>\n");
	print("service: [echoservice], [namedpipe], [game]\n");
	print("mode: [start], [stop], [pause], [resume]\n");
	print("e.g : main echoservice start\n");
	exit(EXIT_FAILURE);
}

int service = chooseService(argv[1]);
int action = chooseAction(argv[2]);
char* state = currentState();

if (service == EXIT_FAILURE || action == EXIT_FAILURE) {
	return EXIT_FAILURE;
}

if (strcmp(action, state)) {
	printf("Service: [%s] befindet sicher bereits in Modus: [%s]", argv[1], argv[2]);
	return EXIT_FAILURE;
}

if (!performStateSwitch(service, action)) {
	printf("Service: [%s] kann Zustand nicht von [%s] in [%s] wechseln.", argv[1], state, argv[2]);
	return EXIT_FAILURE;
}

int start(int port) {
	SOCKET workers[20];
	char* buff = (char*)calloc(BUFFER_SIZE, sizeof(char));
	char* message = (char*)calloc(sizeof(stdReturnMessage) + sizeof(buff), sizeof(char));
	SOCKET tcpListener = NULL;
	tcpListener = createSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bindSocket(&tcpListener, INADDR_ANY, port);
	listenSocket(&tcpListener);
	SOCKADDR_IN serverAddress;

	FD_SET fdset;
	FD_ZERO(&fdset);
	int returnCode = 0;



	for (int i = 0; i < MAX_WORKER; i++) {
		workers[i] = INVALID_SOCKET;
	}

	while (1) {
		//Initializing and Reinitializing of the FS_Set because of modifing through System Calls
		FD_ZERO(&fdset);
		FD_SET(tcpListener, &fdset);
		for (int i = 0; i < MAX_WORKER; i++) {
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
			for (int i = 0; i < MAX_WORKER; i++) {
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
			for (int i = 0; i <= MAX_WORKER; i++) {
				if (FD_ISSET(workers[i], &fdset)) {
					if (buff) {
						returnCode = recv(workers[0], buff, BUFFER_SIZE, 0);
					}
					if (returnCode == 0) {
#ifdef __unix__ || __linux__
						close(worker[i]);
#elif __MSDOS__ || __WIN32__ || _MSC_VER
						closesocket(workers[i]);
#endif
						workers[i] = INVALID_SOCKET;
						printf("Connection with Client [%d] has been closed\n", i);
					}
					else {
						if (returnCode < BUFFER_SIZE - 1) {
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

int stop() {
}

int pause() {
}

int resume() {
}

int chooseService(char* serviceName) {
	if (strcmp(serviceName, TCP_ECHO_SERVICE)) {
		return TCP_ECHO_SERVICE_SERVICE;
	}
	else if (strcmp(serviceName, NAMED_PIPE)) {
		return NAMED_PIPE_SERVICE;
	}
	else if (strcmp(serviceName, UDP_DATE)) {
		return UDP_DATE_SERVICE;
	}
	else {
		return EXIT_FAILURE;
	}
}

/*
 * Evaluates the second input-parameter if it is start,stop,pause or resume
 */
int chooseAction(char* action) {
	if (strcmp(action, "start")) {
	}
	else if (strcmp(action, "stop")) {
	}
	else if (strcmp(action, "pause")) {
	}
	else if (strcmp(action, "resume")) {
	}
	else {
		return EXIT_FAILURE;
	}
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