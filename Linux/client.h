#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <alsa/asoundlib.h>
#include "linuxsocket.h"
#include "soundService.h"

#define END  0
#define ECHOSERVER 1
#define SOUND 2
#define MAX_MESSAGE_SIZE 32

extern void start();
extern int dialog();
extern int getInt(const char* message);
extern void echoServiceHandle();
extern void soundServiceHandle();
extern void setServerAddress(struct sockaddr_in* server);

#endif //CLIENT_H
