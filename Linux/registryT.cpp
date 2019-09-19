#include "registryT.hpp"

snd_pcm_t *pcmHandle;
using namespace std;

void dialog(){
	printf("\n==========================");
	printf("\nServer Commands: \n");
	printf("\n%i. Shutdown server \n", STOP);
	printf("\n%i. Communicate through named-pipe\n", PIPES);
	printf("\n%i. Close connection\n", CANCEL);
	printf("\n==========================\n");
}

void executeHandleEchoServiceThread(int& socket){
	while(!handleEchoService(&socket));
}

void executeTcpThread(int *tcpListener){
	const char *stdWelcomeMessage = "Welcome to Echo-Server v1.0\n";
	int workers[MAX_WORKER];
	thread threadPool[MAX_WORKER];
	//Initialize the worker sockets
	for (int i = 0; i < MAX_WORKER; i++){
		workers[i] = -1;
	}
	for (int i = 0; i < MAX_WORKER; i++){
		if (workers[i] == -1){
				initEchoService(tcpListener, &workers[i], stdWelcomeMessage);
				threadPool[i] = thread(executeHandleEchoServiceThread, ref(workers[i]));
				threadPool[i].join();
		}
	}
}

void executeUdpThread(int *udpListener){
	printf("\nHandle udp-listener : [%d]\n", *udpListener);
	while(1){
		handleSoundService(*udpListener, pcmHandle);
	}
}


int startConnectionHandle(int *tcpListener, int *udpListener, const char *path){

	thread tcpListenThread(executeTcpThread, ref(tcpListener));
	thread udpListenerThread(executeUdpThread, ref(udpListener));
	thread pipeThread(executeTcpThread, ref(tcpListener));

	//tcpListenThread.join();
	udpListenerThread.join();
	pipeThread.join();
}

int initSound(){
	unsigned int samples = 44100;
	char *deviceName = "default";
	int channels = 1;
	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
	snd_pcm_access_t mode = SND_PCM_ACCESS_RW_INTERLEAVED;
	snd_pcm_uframes_t frames = 32;
	if (initSoundDevice(&pcmHandle, deviceName, channels, &samples, stream, format, mode, frames) == EXIT_FAILURE)
	{
		printf("Sound Service not available\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int initTcpSocket(int port){
	int tcpSocket;
	if ((tcpSocket = createSocket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}else{
		printf("Tcp-listener is now at fd : [%d]\n", tcpSocket);
	}if (bindSocket(&tcpSocket, INADDR_ANY, port) < 0){
		return -2;
	}else{
		printf("Tcp-listener is now bound\n");
	}

	if (listenSocket(&tcpSocket) < 0)
	{
		return -3;
	}
	else
	{
		printf("Tcp-listener is now in listen mode\n");
	}
	return tcpSocket;
}

//initialzie the upd-Socket for audio streaming
int initUdpSocket(int port){
	int udpSocket;
	if ((udpSocket = createSocket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		return -1;
	}
	else
	{
		printf("Udp-listener is now at fd : [%d]\n", udpSocket);
	}

	if (bindSocket(&udpSocket, INADDR_ANY, port))
	{
		return -2;
	}
	else
	{
		printf("Udp-listener is now bound\n");
	}
	return udpSocket;
}

int initPipe(const char *path, int permission){
	if ((initNamedPipe(path, permission)) == EEXIST)
	{
		printf("Pipe already exists\n");
		return EEXIST;
	}
	else
	{
		printf("Pipe created\n");
	}
	return EXIT_SUCCESS;
}