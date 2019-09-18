#include "registry.hpp"

const char* stdWelcomeMessage = "Welcome to Echo-Server v1.0\n";
fd_set fdset;
int tcpListener;
int udpListener;
int pipeListener;
snd_pcm_t* pcmHandle;

std::thread workers[MAX_WORKER];

int initializeServer(int tcpPort, int udpPort, const char* path, int permission){
	unsigned int samples = 44100;
	char* deviceName = "default";
	int channels = 1;
 	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
   	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
  	snd_pcm_access_t mode = SND_PCM_ACCESS_RW_INTERLEAVED;
	snd_pcm_uframes_t frames = 32;

	//initialize the tcp-Socket for echo-service
	if((tcpListener = createSocket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}
	printf("Tcp-listener is now at fd : [%d]\n", tcpListener);

	if(bindSocket(&tcpListener, INADDR_ANY, tcpPort)){
		return EXIT_FAILURE;
	}
	printf("Tcp-listener is now bound\n");

	if(listenSocket(&tcpListener)){
		return EXIT_FAILURE;
	}
	printf("Tcp-listener is now in listen mode\n");

	//initialzie the upd-Socket for audio streaming
	if((udpListener = createSocket(AF_INET, SOCK_DGRAM, 0)) < 0){
		return -1;
	}
	printf("Udp-listener is now at fd : [%d]\n", udpListener);

	if(bindSocket(&udpListener, INADDR_ANY, udpPort)){
		return EXIT_FAILURE;
	}
	printf("Udp-listener is now bound\n");

	//Initialize the worker sockets
	for (int i = 0; i < MAX_WORKER; i++) {
		workers[i] = std::thread(excec, i);
	}

	if((initNamedPipe(path, permission))==EEXIST){
		printf("Pipe already exists\n");
	}else{
		printf("Pipe created\n");
	}

	openPipeService(&pipeListener, path);

	if(initSoundDevice(&pcmHandle, deviceName,channels,&samples, stream, format, mode, frames) == EXIT_FAILURE){
		printf("Sound Service not available\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
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
	std::thread tcpListener();
	std::thread udpListener();
	std::thread tcpListener();
	
	dialog();
	while (1) {
		
		STDIN_FILENO
		handleNamedPipeServiceWrite(workers[i]);
		handleSoundService(udpListener, pcmHandle)
		handleNamedPipeServiceRead(pipeListener);
		initEchoService(&tcpListener, &workers[i], stdWelcomeMessage);
		handleEchoService(&workers[i]) == EXIT_FAILURE){
				
}

void closeConnection(int* socket){
	shutdown(*socket,SHUT_RDWR);
	*socket = -1;
	printf("\nConnection has been shutdown\n");
	dialog();
}

