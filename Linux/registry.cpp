#include "registry.h"

snd_pcm_t *pcmHandle;
mutex closeMutex;
mutex workerMutex;
mutex downMutex;
mutex pipeMutex;
bool down = false;
int kill = -2;
bool writeToPipe = false;
bool connectionClose = false;

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

/*
 * @function	dialog
 * @abstract	A dialog
 * @discuss 	This function prints the posibilities of the Serverinteraction
 * @result		Results an Integer
 */
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


/*
 * @function	executeHandleEchoServiceThread
 * @abstract	Handles the Echo Thread client communication
 * @discuss 	This function is responsible for the execution of the Echo Service handle
 * @param		socket	Pointer-Id of the Worker-socket
 */
void executeHandleEchoServiceThread(int& socket){
	int rt;
	fcntl(socket, F_SETFL, O_NONBLOCK);
	while(!down || kill==socket){
	timer stopwatch;
		rt = handleEchoService(&socket);
		if( rt == -1 || rt == EXIT_FAILURE){
			break;
		}else if(rt == EXIT_SUCCESS){
			cout << "Thread with worker : " << socket << " took " << stopwatch.elapsed() << endl;
		}else{
			sleep(1);
		}
		
	}
	cout << "Echo client thread exits : " << &socket << endl;
}


/*
 * @function	executeTcpThread
 * @abstract	Handles the Echo Thread
 * @discuss 	This function is responsible for the execution of the Echo Service
 * @param		tcpListener	Pointer-Id of the Tcp-Listener-socket
 */
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
					unique_lock<mutex> lock(workerMutex);
					if(initEchoService(&tcpListener, &workers[i], stdWelcomeMessage) == EXIT_SUCCESS){	
						threadPool[i] = thread(executeHandleEchoServiceThread, ref(workers[i]));
					}else{
						sleep(1);
					}
					if(down)
						break;
					if(connectionClose){
						for (int i = 0; i < MAX_WORKER; i++){
							if(workers[i]==kill){
								unique_lock<mutex> lock(closeMutex);
								closeConnection(&workers[i]);
								connectionClose = false;
								kill=-2;
								lock.unlock();
							}
						}
					}
				lock.unlock();
			}
		}
	}
	
	for (int i = 0; i < MAX_WORKER; i++){
		if(workers[i] != -1){
			threadPool[i].join();
		}
	}
	cout << "Tcp Thread exits" << endl;
    timerTcpThread=stopwatch.elapsed();
}


/*
 * @function	executeUdpThread
 * @abstract	Handles the Sound Thread
 * @discuss 	This function is responsible for the execution of the Sound Service
 * @param		udpListener	Pointer-Id of the Udp-Listener-socket
 */
void executeUdpThread(int udpListener){
	timer stopwatch;
	while(!down){
		handleSoundService(udpListener, pcmHandle);
	}
	closeSoundService(pcmHandle);
	cout << "Udp Thread exits" << endl;
    timerUdpThread=stopwatch.elapsed();
}

/*
 * @function	executePipeThreadRead
 * @abstract	Handles the Pipe Thread
 * @discuss 	This function is responsible for the execution of the Pipe Service
 * @param		pipeListener	Pointer-Id of the pipeListener-socket
 * @param		path			Path of the Named Pipe
 */
void executePipeThreadRead(int& pipeListener,const char* path){
	timer stopwatch;
	while(!down){
		if(!writeToPipe){
			unique_lock<mutex> lock(pipeMutex);
			handleNamedPipeServiceRead(&pipeListener, path);
			lock.unlock();
		}
	}
	cout << "Pipe Thread exits" << endl;
    timerPipeThread=stopwatch.elapsed();
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
int startConnectionHandle(int *tcpListener, int *udpListener, const char *path){
	timer stopwatch;
	int command;
	int pipeListener = open(path, O_RDONLY | O_NONBLOCK);
	if (pipeListener < 0){
		cout << "pipe could no be opened : [%d]" << pipeListener << endl;
	}
	fcntl(*tcpListener, F_SETFL, O_NONBLOCK);
	fcntl(*udpListener, F_SETFL, O_NONBLOCK);
	thread tcpListenThread(executeTcpThread, *tcpListener);
	thread udpListenerThread(executeUdpThread, *udpListener);
	thread pipeThread(executePipeThreadRead, ref(pipeListener),path);

	while(true){

		command = dialog();
		//command = 1;
		if(command == STOP){
			unique_lock<mutex> lock(downMutex);
			cout << "Triggered server command STOP."<< endl;
			cout << "Server will now shutdown." << endl;
			down = true;
			lock.unlock();
			break;
		}else if(command == PIPES){
			unique_lock<mutex> lock(pipeMutex);
			writeToPipe = true;
			char* message = (char*)calloc(MAX_BUFF_SIZE, sizeof(char));
			cout <<"\n Write to Pipe ~> "; cin >> message;
			handleNamedPipeServiceWrite(-1, &pipeListener, path, message, strlen(message));
			writeToPipe = false;
			free(message);
			lock.unlock();
		}else if(command == CANCEL){
			unique_lock<mutex> lock(closeMutex);
			kill = getInt("\nFile descriptor Id: ");
			connectionClose = true;
			lock.unlock();
		}
	}
	tcpListenThread.join();
	udpListenerThread.join();
	pipeThread.join();
	cout << "Thread execution time : " << timerTcpThread << endl;
	cout << "Thread execution time : " << timerUdpThread << endl;
    timerPipeThread=stopwatch.elapsed();
	cout << "Thread execution time : " << timerAll << endl;
	cout << "All threads closed. Have a nice day!!!" << endl;
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

/*
 * @function	initTcpSocket
 * @abstract	Initialize Tcp-Socket
 * @discuss 	This function initializes the Socket for the Tcp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
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

/*
 * @function	initUdpSocket
 * @abstract	Initialize Udp-Socket for Audio-Streaming
 * @discuss 	This function initializes the Socket for the Udp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
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

/*
 * @function	initPipe
 * @abstract	Initializes the Pipe handle
 * @discuss 	This function configures the server for the communikation with the Pipe
 * @param		path			Path of the Named Pipe
 * @param		permission		Rights of the Pipe
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
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


/*
 * @function	getInt
 * @abstract	String to Int
 * @discuss 	This function converts an Integer as String into an real Integer
 * @param		message			Integer as String
 * @result		Results the converted Number
 */
int getInt(const char* message){
  printf("%s",message);
  long value;
  fflush(stdin);
  fgets((char*)&value, sizeof(value)-1, stdin);
  fflush(stdin);
  value = atoi((char*)&value);
  return value;
}
