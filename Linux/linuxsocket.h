#ifndef SOCKETS_H
#define SOCKETS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

extern int createSocket(int, int, int);
extern void listenSocket(int*);
extern void bindSocket(int* , unsigned int, int);
extern void acceptSocket(int* listener, int* worker);
extern void closeSocket(int* socket);

#endif // SOCKETS_H
