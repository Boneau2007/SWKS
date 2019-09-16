#ifndef PIPESERVICE_H
#define PIPESERVICE_H
#define MAX_BUFF_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <errno.h> 
#include "linuxsocket.h"

extern int initNamedPipe(char* path, mode_t mode);
extern void connectToPipeService(int* worker,char* path, int mode);
extern void handleNamedPipeService();

#endif //PIPESERVICE_H
