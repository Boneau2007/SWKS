#include "registry.h"

const char* stdWelcomeMessage = "Welcome to Echo-Server v1.0";
const char* stdSoundWelcomeMessage = "Welcome to Sound-Server v1.0";
fd_set fdset;

int tcpListener;
int udpListener;
int pipeListener;
snd_pcm_t* pcmHandle;

struct connection workers[MAX_WORKER];

void initializeServer(int tcpPort, int udpPort, const char* path, int permission){
	unsigned int samples = 44100;
	char* deviceName = "default";
	int channels = 1;
 	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
   	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
  	snd_pcm_access_t mode = SND_PCM_ACCESS_RW_INTERLEAVED;
	snd_pcm_uframes_t frames = 32;
	FD_ZERO(&fdset);

	//initialize the tcp-Socket for echo-service
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

	if((initNamedPipe(path, permission))==EEXIST){
		printf("");
	}

	if(initSoundDevice(&pcmHandle, deviceName,channels,&samples, stream, format, mode, frames) == EXIT_FAILURE){
		printf("Sound Service not available.");
		exit(EXIT_FAILURE);
	}
}

void dialog(){
	printf("\n==========================");
	printf("\nServer Commands: \n");
	printf("\n%i. Shutdown server \n",STOP);
	printf("\n%i. Communicate through named-pipe\n",PIPES);
	printf("\n%i. Close connection\n",CANCEL);
	printf("\n==========================\n");
}
int startConnectionHandle() {
	int err;	
	dialog();
	while (1) {
		//Re-/initializing the FS_Set because of modifing through System Calls
		FD_ZERO(&fdset);

		FD_CLR(tcpListener, &fdset);
		FD_CLR(udpListener, &fdset);
		FD_CLR(STDIN_FILENO, &fdset);

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
					if(workers[i].socketId != -1){
						closeConnection(&workers[i]);
					}
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
		//If some event happend on the tcp-socket
		if (FD_ISSET(tcpListener, &fdset)|| FD_ISSET(pipeListener, &fdset)) {
			for (int i = 0; i < MAX_WORKER; i++) {
				if (workers[i].socketId == -1) {
					if((FD_ISSET(tcpListener, &fdset))){
						workers[i].type = ECHO;
						initEchoService(&tcpListener, &workers[i].socketId, stdWelcomeMessage);
					}else if((FD_ISSET(pipeListener, &fdset))){
						workers[i].type = PIPE;
						connectToPipeService(&pipeListener, &workers[i].socketId);
					}
					printf("Worker[%d] is now set \n", workers[i].socketId);
					break;
					
				}
			}
		}
		//Else some I\O happens on a worker-socket descriptors
		else {
			if(FD_ISSET(udpListener, &fdset)){
				handleSoundService(udpListener, pcmHandle);
			}else{
				for (int i = 0; i <= MAX_WORKER; i++) {
					if (FD_ISSET(workers[i].socketId, &fdset)) {
						printf("\nWorker[%d] performes I\\O \n", workers[i].socketId);
						if(workers[i].type == ECHO){
							if(handleEchoService(&workers[i].socketId) == EXIT_FAILURE){
								closeConnection(&workers[i]);
							}
						}else if(workers[i].type == PIPE){
							handleNamedPipeService(&workers[i]);			
						}
					
					}
				}
			}
			
		}
	}
	closeSocket(&tcpListener);
	closeSocket(&udpListener);
	closeSoundService(pcmHandle);
	exit(EXIT_SUCCESS);
}

void closeConnection(struct connection* connection){
	shutdown((*connection).socketId,SHUT_RDWR);
	(*connection).socketId = -1;
	(*connection).type = -1;
	printf("\nConnection has been shutdown\n");
	dialog();
}

