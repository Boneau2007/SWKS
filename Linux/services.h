#ifndef LINUXECHOSERVICE_H
#define LINUXECHOSERVICE_H
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
#define MAX_COMMAND_SIZE 64
#define STOP 1
#define CANCEL 2

enum connectionType {ECHO, SOUND, PIPE} ; 

struct connection{
	int socketId;
	enum connectionType type;
};

void dialog();

void initializeServer(int,int);
int startConnectionHandle();

extern void handleEchoServiceConnect(struct connection* connection);
extern void handleSoundServiceConnect(struct connection* connection);
extern void handleNamedPipeConnect(struct connection* connection, char* path, mode_t mode);

extern void handleEchoService(struct connection* connection);
extern void handleNamedPipeService(struct connection* connection);
extern void handleSoundService(struct connection* connection);

void closeConnection(struct connection* connection);
#endif //LINUXECHOSERVICE_H
