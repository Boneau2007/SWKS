#ifndef REGISTRY_HPP
#define REGISTRY_HPP
#define MAX_WORKER 20
#define BUFF_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>      
#include <errno.h>
#include <alsa/asoundlib.h>
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::future
#include <queue>         // std::queue
#include <mutex>         // std::mutex
#include <atomic>
#include <chrono>


#ifdef __cplusplus
extern "C" {
#endif
#include "linuxsocket.h"
#include "echoService.h"
#include "pipeService.h"
#include "soundService.h"
#ifdef __cplusplus
}
#endif 

#define MAX_COMMAND_SIZE 64
#define STOP 1
#define PIPES 2
#define CANCEL 3


enum pipeType {READ, WRITE} ; 

struct connection{
	int socketId;
	enum pipeType type;
};

using namespace std;
using namespace std::chrono;

extern int startConnectionHandle(int* tcpListener, int* udpListener, const char* path);
extern int initTcpSocket(int port);
extern int initUdpSocket(int port);
extern int initPipe(const char*, int);
extern int initSound();

extern void executeTcpThread(int tcpListener);
extern void executeUdpThread(int udpListener);
extern void executePipeThreadRead(const char * path);
extern void echoService(int* tcpListener);
extern int getInt(const char* message);
extern void closeConnection(int* socket);
#endif //REGISTRY_HPP
