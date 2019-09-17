#ifndef REGISTRY_H
#define REGISTRY_H
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

#include "linuxsocket.h"
#include "echoService.h"
#include "pipeService.h"
#include "soundService.h"

#define MAX_COMMAND_SIZE 64
#define STOP 1
#define PIPES 2
#define CANCEL 3

enum pipeType {READ, WRITE} ; 

struct connection{
	int socketId;
	enum pipeType type;
};

extern void dialog();
extern int initializeServer(int,int,const char*, int);
extern int startConnectionHandle();
extern void closeConnection(int* socket);

#endif //REGISTRY_H
