#ifndef ECHOSERVICE_H
#define ECHOSERVICE_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "linuxsocket.h"
#define BUFF_SIZE 256

extern void initEchoService(int* listenerSocket, int* workerSocket, const char* message);
extern int handleEchoService(int* workerSocket);

#endif //ECHOSERVICE_H
