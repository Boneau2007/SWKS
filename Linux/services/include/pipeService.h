#ifndef PIPESERVICE_H
#define PIPESERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <errno.h>

#define MAX_BUFF_SIZE 256

extern int init(const char* path, mode_t mode);
extern void readHandle(int* reader, const char* path);
extern void writeHandle(int worker, int* reader, const char* path, const char* message, int length);

#endif //PIPESERVICE_H
