#ifndef PIPESERVICE_H
#define PIPESERVICE_H
#define MAX_WORKER 20
#define BUFF_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>    
#include <errno.h>
#include <alsa/asoundlib.h>
#include "linuxsocket.h"

extern void initNamedPipe(char* path, mode_t mode);
extern void handleNamedPipeService();

#endif //PIPESERVICE_H
