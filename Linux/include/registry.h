#ifndef REGISTRY_HPP
#define REGISTRY_HPP
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <alsa/asoundlib.h>
#ifdef __cplusplus
#include <string>
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::future
#include <queue>         // std::queue
#include <mutex>         // std::mutex
#include <atomic>
#include <chrono>
extern "C" {
#endif
#include "linuxsocket.h"
#include "echoService.h"
#include "pipeService.h"
#include "soundService.h"
#ifdef __cplusplus
}
using namespace std;
using namespace std::chrono;
#endif 

#define MAX_WORKER 20
#define BUFF_SIZE 256
#define MAX_COMMAND_SIZE 64
#define STOP 1
#define PIPES 2
#define CANCEL 3

extern int startConnectionHandle(int* tcpListener, int* udpListener, const char* path);
extern int initTcpSocket(int port);
extern int initUdpSocket(int port);
extern int initPipe(const char*, int);
extern int initSound();

#ifdef __cplusplus
extern void executeHandleEchoServiceThread(int& socket);
extern void executeTcpThread(int tcpListener);
extern void executeUdpThread(int udpListener);
extern void executePipeThreadRead(int& pipeListener,const char * path);
extern int getInt(const char* message);
#endif 
#endif //REGISTRY_HPP
