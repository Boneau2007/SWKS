#include "registryT.hpp"

snd_pcm_t *pcmHandle;
mutex serverCommandMutex;
bool down = false;
bool writeToPipe = false;
bool connectionClose = false;

double timerResultEchoHandle;
double timerTcpThread;

double timerUdpThread;
double timerPipeThread;
double timerAll;

class timer {
public:
    time_point<high_resolution_clock> lastTime;
    timer() : lastTime(high_resolution_clock::now()) {}
    inline double elapsed() {
        time_point<high_resolution_clock> thisTime=high_resolution_clock::now();
        double deltaTime = duration<double>(thisTime-lastTime).count();
        lastTime = thisTime;
        return deltaTime;
    }
};

int dialog(){
	int command;
	printf("\n==========================");
	printf("\nServer Commands: \n");
	printf("\n%i. Shutdown server \n", STOP);
	printf("\n%i. Communicate through named-pipe\n", PIPES);
	printf("\n%i. Close connection\n", CANCEL);
	printf("\n==========================\n");
	command = getInt("Enter Value: ");
	return command;
}

void executeHandleEchoServiceThread(int& socket){
	timer stopwatch;
	int rt;
	fcntl(socket, F_SETFL, O_NONBLOCK);
	while(!down){
		rt = handleEchoService(&socket);
		if( rt == -1 || rt == EXIT_FAILURE){
			break;
		}else{
			sleep(2);
		}
	}
	cout << "Worker handle Thread exits" << endl;
    timerResultEchoHandle=stopwatch.elapsed();
}

void executeTcpThread(int tcpListener){
	timer stopwatch;
	const char *stdWelcomeMessage = "Welcome to Echo-Server v1.0\n";
	int workers[MAX_WORKER];
	thread threadPool[MAX_WORKER];
	//Initialize the worker sockets
	for (int i = 0; i < MAX_WORKER; i++){
		workers[i] = -1;
	}
	while(!down){
	for (int i = 0; i < MAX_WORKER; i++){
		if (workers[i] == -1){
				if(initEchoService(&tcpListener, &workers[i], stdWelcomeMessage) == EXIT_SUCCESS){	
					threadPool[i] = thread(executeHandleEchoServiceThread, ref(workers[i]));
					threadPool[i].join();
				}else{
					cout << "Con. pending, sleep 5 seconds" << endl;
					//sleep(5);
				}
				if(down)
					break;
				if(connectionClose){
					int fd = getInt("\nFile descriptor Id: ");
					closeConnection(&fd);
				}
		}
	}
	}
	cout << "Tcp Thread exits" << endl;
    timerTcpThread=stopwatch.elapsed();
}

void executeUdpThread(int udpListener){
	timer stopwatch;
	while(!down){
		handleSoundService(udpListener, pcmHandle);
	}
	closeSoundService(pcmHandle);
	cout << "Udp Thread exits" << endl;
    timerUdpThread=stopwatch.elapsed();
}

void executePipeThreadRead(const char* path){
	timer stopwatch;
	int pipeListener = open(path, O_RDONLY | O_NONBLOCK);
	if (pipeListener < 0){
		cout << "pipe could no be opened : [%d]" << pipeListener << endl;
	}
	while(!down){
		if(!writeToPipe){
			handleNamedPipeServiceRead(&pipeListener, path);
		}else{
			handleNamedPipeServiceWrite(pipeListener, &pipeListener, path);
			unique_lock<mutex> lock(serverCommandMutex);
			writeToPipe = false;
			lock.unlock();
		}
	}
	cout << "Pipe Thread exits" << endl;
    timerPipeThread=stopwatch.elapsed();
}

int startConnectionHandle(int *tcpListener, int *udpListener, const char *path){
	timer stopwatch;
	int command;

	fcntl(*tcpListener, F_SETFL, O_NONBLOCK);
	fcntl(*udpListener, F_SETFL, O_NONBLOCK);
	thread tcpListenThread(executeTcpThread, *tcpListener);
	thread udpListenerThread(executeUdpThread, *udpListener);
	thread pipeThread(executePipeThreadRead, path);

	while(1){
		command = dialog();
		command = 1;
		if(command == STOP){
			unique_lock<mutex> lock(serverCommandMutex);
			cout << "Triggered server command STOP."<< endl;
			cout << "Server will now shutdown." << endl;
			down = true;
			lock.unlock();
			break;
		}else if(command == PIPES){
			unique_lock<mutex> lock(serverCommandMutex);
			writeToPipe = true;
			lock.unlock();
		}else if(command == CANCEL){
			
		}
	}
	tcpListenThread.join();
	udpListenerThread.join();
	pipeThread.join();
	cout << "Thread execution time : " << timerResultEchoHandle << endl;
	cout << "Thread execution time : " << timerTcpThread << endl;
	cout << "Thread execution time : " << timerUdpThread << endl;
	cout << "Thread execution time : " << timerPipeThread << endl;
    timerPipeThread=stopwatch.elapsed();
	cout << "Thread execution time : " << timerAll << endl;
	cout << "All threads closed. Have a nice day!!!" << endl;
	return EXIT_SUCCESS;
}

int initSound(){
	unsigned int samples = 44100;
	const char *deviceName = "default";
	int channels = 1;
	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
	snd_pcm_access_t mode = SND_PCM_ACCESS_RW_INTERLEAVED;
	snd_pcm_uframes_t frames = 32;
	if (initSoundDevice(&pcmHandle, deviceName, channels, &samples, stream, format, mode, frames) == EXIT_FAILURE)
	{
		cout << "Sound Service not available" << endl;
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


int getInt(const char* message){
  printf("%s",message);
  long value;
  fflush(stdin);
  fgets((char*)&value, sizeof(value)-1, stdin);
  fflush(stdin);
  value = atoi((char*)&value);
  return value;
}
