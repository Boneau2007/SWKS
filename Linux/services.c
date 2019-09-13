#include "linuxsocket.h"
#include "services.h"

const char* stdWelcomeMessage = "Welcome to Echo-Server v1.0";
char* stdReturnMessage = "Server answered: ";
fd_set fdset;

int tcpListener;
int udpListener;
struct connection workers[MAX_WORKER];

void initializeServer(int tcpPort, int udpPort){

	FD_ZERO(&fdset);

	//initialize the tcp-Socket for echo service
	tcpListener = createSocket(AF_INET, SOCK_STREAM, 0);
	bindSocket(&tcpListener, INADDR_ANY, tcpPort);
	listenSocket(&tcpListener);

	//initialzie the upd-Socket for audio streaming
	udpListener = createSocket(AF_INET, SOCK_DGRAM, 0);
	bindSocket(&udpListener, INADDR_ANY, udpPort);

	//Initialize the worker sockets
	for (int i = 0; i < MAX_WORKER; i++) {
		workers[i].socketId = -1;
	}
}

void dialog(){
	printf("\n==========================");
	printf("\nServer Commands: \n");
	printf("\n%i. Shutdown server \n",STOP);
	printf("\n%i. Close connection\n",CANCEL);
	printf("\n==========================");
  	printf("\nEnter Value: ");
  	fflush(stdout);
}
int startConnectionHandle() {
	int err;	
	dialog();
	while (1) {
		//Re-/initializing the FS_Set because of modifing through System Calls
		FD_ZERO(&fdset);
		FD_CLR(tcpListener, &fdset);
		FD_SET(tcpListener, &fdset);
		FD_SET(udpListener, &fdset);
		FD_SET(STDIN_FILENO, &fdset);
		for (int i = 0; i < MAX_WORKER; i++) {
			if (workers[i].socketId != -1) {
				FD_SET(workers[i].socketId, &fdset);
			}
		}
		interrupt:
		if ((err = select(FD_SETSIZE, &fdset, NULL, NULL, NULL) )< 0) {
			if (err < 0) {
        		switch (errno) {
        		case EINTR:     goto interrupt;
        		case EAGAIN:    break; /* empty rx queue */
        		case ETIMEDOUT: break; /* recv timeout */
        		case ENOTCONN:  break; /* not connected yet */
        		default:        printf("Connetction lost [%d]", err);
        		}
			}
		}
		if(FD_ISSET(STDIN_FILENO, &fdset)){
			int command;
  			fgets((char*)&command, 8, stdin);
			fflush(stdin);
  			command = atoi((char*)&command);
			if(command == STOP){
				printf("\nTriggered server command STOP.");
				printf("\nServer will now shutdown.\n");
				for(int i = 0;i < MAX_WORKER;i++){
					closeConnection(&workers[i]);
				}
				break;
			}else if(command == CANCEL){
				int fd;
				printf("\nFile descriptor Id: ");
  				fgets((char*)&fd, 8, stdin);
			  	fflush(stdin);
  				fd = atoi((char*)&fd);
				if(workers[fd].socketId < 0){
					printf("\nNo connection -> worker [%d]\n", fd);
				}else{
					closeConnection(&workers[fd]);
				}
			}
			dialog();
		}
		//If some event happend on the tcp- or udp-socket 
		if (FD_ISSET(tcpListener, &fdset) || (FD_ISSET(udpListener, &fdset))) {
			for (int i = 0; i < MAX_WORKER; i++) {
				if (workers[i].socketId == -1) {
					if((FD_ISSET(tcpListener, &fdset))){
						handleEchoServiceConnect(&workers[i]);
					}else if((FD_ISSET(udpListener, &fdset))){
						handleSoundServiceConnect(&workers[i]);
					}
					printf("Worker[%d] is now set \n", workers[i].socketId);
					break;
					
				}
			}
		}
		//Else some I\O happens on a worker-socket descriptors
		else {
			for (int i = 0; i <= MAX_WORKER; i++) {
				if (FD_ISSET(workers[i].socketId, &fdset)) {
					printf("\nWorker[%d] performes I\\O \n", workers[i].socketId);
					if(workers[i].type == ECHO){
						handleEchoService(&workers[i]);
					}else if(workers[i].type == SOUND){
						handleSoundService(&workers[i]);
					}else if(workers[i].type == PIPE){
						handleNamedPipeService(&workers[i]);
						
					}
					
				}
			}
		}
	}
	closeSocket(&tcpListener);
	closeSocket(&udpListener);
	exit(EXIT_SUCCESS);
}

void closeConnection(struct connection* connection){
	shutdown((*connection).socketId,SHUT_RDWR);
	(*connection).socketId = -1;
	(*connection).type = -1;
	printf("\nConnection has been shutdown");
}

void handleEchoServiceConnect(struct connection*  connection){
	(*connection).type = ECHO;
	acceptSocket(&tcpListener, &(*connection).socketId);
	printf("\nSending welcome message.\n");
	if (write((*connection).socketId, stdWelcomeMessage, strlen(stdWelcomeMessage)) == -1){
		printf("\nError, while try to send.\n");
	}else{
		printf("\nWelcome message has been send.\n");
	}
}

/*
 * This function handles the tcp communication of the echo service
 */
void handleEchoService(struct connection* connection){
	char* buff = (char*)calloc(BUFF_SIZE, sizeof(char));
	char* message = (char*)calloc(BUFF_SIZE, sizeof(char));
	int returnCode = 0;
	if (buff) {
		returnCode = recv((*connection).socketId, buff, BUFF_SIZE, 0);
		printf("Worker[%d] recieved [%d] bytes \n", (*connection).socketId, returnCode);
		if(strncmp(buff, "CLOSE",5)==0){
			printf("Worker[%d] recieved a CLOSE call. \n", (*connection).socketId);
			returnCode = 0;
		}
	}
	if (returnCode == 0) {
		//Shutdown all communication
		closeConnection(connection);
	}else {
		if (returnCode < BUFF_SIZE - 1) {
			if (buff) {
				strcpy(message, stdReturnMessage);
				strcat(message, buff);
				printf("Send Message at worker : [%d] \n", connection->socketId);
				write((*connection).socketId, message, strlen(message));
				printf("Message has been send\n");
				memset(buff,'\0', sizeof(char)*BUFF_SIZE);
				memset(message,'\0', sizeof(char)*BUFF_SIZE);
			}
		}
	}
	free(buff);
	free(message);
}


void handleSoundServiceConnect(struct connection* connection){
}

void handleSoundService(struct connection* connection) {
}


void handleNamedPipeConnect(struct connection*  connection, char* path, mode_t mode){
	if(mkfifo(path, mode) == -1){
		printf("Couldn't create fifo\n");
	}
}
void handleNamedPipeService(struct connection* connection) {
}