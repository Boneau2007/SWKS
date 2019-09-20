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
#include <errno.h>

extern int createSocket(int family, int type, int protocol);
extern int listenSocket(int* socket);
extern int bindSocket(int* socket, unsigned int address, int port);
extern int acceptSocket(int* listener, int* worker);
extern int closeSocket(int* socket);

#endif // SOCKETS_H
