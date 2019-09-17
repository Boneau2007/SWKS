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


extern void itoa(int input, char *buffer, int radix);
extern int initNamedPipe(const char* path, mode_t mode);
extern void openPipeService(int* reader, const char* path);
extern void handleNamedPipeServiceRead(int * worker);
extern void handleNamedPipeServiceWrite(int * worker);

#endif //PIPESERVICE_H
