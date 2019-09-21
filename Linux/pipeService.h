#ifndef PIPESERVICE_H
#define PIPESERVICE_H
#define MAX_BUFF_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <errno.h>

extern int initNamedPipe(const char* path, mode_t mode);
extern void handleNamedPipeServiceRead(int* reader, const char* path);
extern void handleNamedPipeServiceWrite(int worker,int* reader, const char* path, const char* message, int length);

#endif //PIPESERVICE_H
