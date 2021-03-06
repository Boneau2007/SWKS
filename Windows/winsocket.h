#ifndef SOCKETS_H
#define SOCKETS_H
#include <sys/types.h>
#include <time.h>
#include <io.h>
#include <WinSock2.h> //FD_SET, FD_ISSET, FD_ZERO macros for WIN
#include <Windows.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
extern int initWinSocket();
extern SOCKET createSocket(int, int, int);
extern void listenSocket(SOCKET*);
extern void bindSocket(SOCKET* , unsigned long, int);
extern void acceptSocket(SOCKET* , SOCKET*);
extern void concatString(char*, char*);

#endif // SOCKETS_H
