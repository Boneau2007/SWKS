#ifndef ECHOSERVICE_H
#define ECHOSERVICE_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "linuxsocket.h"
#define BUFF_SIZE 256

extern int initEchoService(int* listener, int* worker, const char* message);
extern int handleEchoService(int* socket);
extern void closeConnection(int* socket);
#endif //ECHOSERVICE_H
