#include "registry.h"

snd_pcm_t* pcmHandle;


/*
 * @function	initTcpSocket
 * @abstract	Initialize Tcp-Socket
 * @discuss 	This function initializes the Socket for the Tcp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int initTcpSocket(int port){
	int tcpSocket;
	if((tcpSocket = createSocket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}else{
		printf("Tcp-listener is now at fd : [%d]\n", tcpSocket);
	}
	
	if(bindSocket(&tcpSocket, INADDR_ANY, port) < 0){
		return -2;
	}else{
		printf("Tcp-listener is now bound\n");
	}

	if(listenSocket(&tcpSocket) < 0){
		return -3;
	}else{
		printf("Tcp-listener is now in listen mode\n");	
	}
	return tcpSocket;
}

/*
 * @function	initUdpSocket
 * @abstract	Initialize Udp-Socket for Audio-Streaming
 * @discuss 	This function initializes the Socket for the Udp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int initUdpSocket(int port){
	int udpSocket;
	if((udpSocket = createSocket(AF_INET, SOCK_DGRAM, 0)) < 0){
		return -1;
	}else{
		printf("Udp-listener is now at fd : [%d]\n", udpSocket);
	}

	if(bindSocket(&udpSocket, INADDR_ANY, port)){
		return -2;
	}else{
		printf("Udp-listener is now bound\n");
	}
	return udpSocket;
}


/*
 * @function	initPipe
 * @abstract	Initializes the Pipe handle
 * @discuss 	This function configures the server for the communikation with the Pipe
 * @param		path			Path of the Named Pipe
 * @param		permission		Rights of the Pipe
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int initPipe(const char* path, int permission){
	if((initNamedPipe(path, permission))==EEXIST){
		printf("Pipe already exists\n");
		return EEXIST;
	}else{
		printf("Pipe created\n");
	}
	return EXIT_SUCCESS;
}


/*
 * @function	initSound
 * @abstract	Initializes the Sound handle
 * @discuss 	This function configures the soundcard for use
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int initSound(){
	unsigned int samples = 44100;
	char* deviceName = "default";
	int channels = 1;
 	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
   	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
  	snd_pcm_access_t mode = SND_PCM_ACCESS_RW_INTERLEAVED;
	snd_pcm_uframes_t frames = 32;
	if(initSoundDevice(&pcmHandle, deviceName,channels,&samples, stream, format, mode, frames) == EXIT_FAILURE){
		printf("Sound Service not available\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


/*
 * @function	dialog
 * @abstract	A dialog
 * @discuss 	This function prints the posibilities of the Serverinteraction
 */
void dialog(){
	printf("\n==========================");
	printf("\nServer Commands: \n");
	printf("\n%i. Shutdown server \n",STOP);
	printf("\n%i. Communicate through named-pipe\n",PIPES);
	printf("\n%i. Close connection\n",CANCEL);
	printf("\n==========================\n");
}

/*
 * @function	startConnectionHandle
 * @abstract	Startes the Server
 * @discuss 	This function handles all in and output of the Server
 * @param		tcpListener		Pointer-Id of the Tcp-Listener-socket
 * @param		udpListener		Pointer-Id of the Udp-Listener-socket
 * @param		path			Path of the Named Pipe
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int startConnectionHandle(int* tcpListener, int* udpListener, const char* path) {
	const char* stdWelcomeMessage = "Welcome to Echo-Server v1.0\n";
	fd_set fdset;
	int err;
	int workers[MAX_WORKER];
	int pipeListener = open(path, O_RDONLY | O_NONBLOCK);
	if (pipeListener < 0){
		printf("pipe could no be opened : [%d]", errno);
		return errno;
	}
	//Initialize the worker sockets
	for (int i = 0; i < MAX_WORKER; i++) {
		workers[i] = -1;
	}
	fcntl(*tcpListener, F_SETFL, O_NONBLOCK);
	dialog();
	while (1) {
		//Re-/initializing the FS_Set because of modifing through System Calls
		FD_ZERO(&fdset);

		FD_CLR(*tcpListener, &fdset);
		FD_CLR(*udpListener, &fdset);
		FD_CLR(pipeListener, &fdset);
		FD_CLR(STDIN_FILENO, &fdset);

		FD_SET(*tcpListener, &fdset);
		FD_SET(*udpListener, &fdset);
		FD_SET(pipeListener, &fdset);
		FD_SET(STDIN_FILENO, &fdset);
		for (int i = 0; i < MAX_WORKER; i++) {
			if (workers[i] != -1) {
				FD_SET(workers[i], &fdset);
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

		//Handles STDIN-I/O
		if(FD_ISSET(STDIN_FILENO, &fdset)){
			int command;
  			fgets((char*)&command, 8, stdin);
			fflush(stdin);
  			command = atoi((char*)&command);
			if(command == STOP){
				printf("\nTriggered server command STOP.");
				printf("\nServer will now shutdown.\n");
				for(int i = 0;i < MAX_WORKER;i++){
					if(workers[i] != -1){
						closeConnection(&workers[i]);
					}
				}
				break;
			}else if(command == PIPES){
				for(int i = 0;i < MAX_WORKER;i++){
					if(workers[i] == -1){
						char* message = (char*)calloc(MAX_BUFF_SIZE, sizeof(char));
						printf("\n Write to Pipe ~>");
						fgets(message, MAX_BUFF_SIZE, stdin);
						fflush(stdin);
						handleNamedPipeServiceWrite(-1, &pipeListener, path, message, strlen(message));
						free(message);
						break;
					}
				}
			}else if(command == CANCEL){
				int fd;
				printf("\nFile descriptor Id: ");
  				fgets((char*)&fd, 8, stdin);
			  	fflush(stdin);
  				fd = atoi((char*)&fd);
				if(workers[fd] < 0){
					printf("\nNo connection -> worker [%d]\n", fd);
				}else{
					closeConnection(&workers[fd]);
				}
			}
			dialog();

		}else if(FD_ISSET(*udpListener, &fdset)){
				handleSoundService(*udpListener, pcmHandle);
		}else if(FD_ISSET(pipeListener, &fdset)){
				handleNamedPipeServiceRead(&pipeListener, path);
		}else if (FD_ISSET(*tcpListener, &fdset)) {
			for (int i = 0; i < MAX_WORKER; i++) {
				if (workers[i] == -1) {
					initEchoService(tcpListener, &workers[i], stdWelcomeMessage);
					printf("Worker[%d] is now set \n", workers[i]);
					break;
				}
			}
		}
		//Else some I\O happens at a worker-socket descriptors
		else {
			for (int i = 0; i < MAX_WORKER; i++) {
				if (FD_ISSET(workers[i], &fdset)) {
					clock_t time = clock();
					if(handleEchoService(&workers[i]) == EXIT_FAILURE){
						closeConnection(&workers[i]);
					}
					time = clock() - time;double 
					time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds 
					fprintf(stdout,"Connection took : [%f]\n", time_taken);
				}
			}
		}
	}
	closeSocket(tcpListener);
	closeSocket(udpListener);
	closeSoundService(pcmHandle);
	return EXIT_SUCCESS;
}
