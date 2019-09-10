#ifndef SOCKETS_H
#define SOCKETS_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
extern int createSocket(int, int, int);
extern void listenSocket(int);
extern void bindSocket(int , unsigned int, int);
extern void acceptSocket(int , int);

#endif // SOCKETS_H
