#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#define END  0
#define ECHOSERVER 1
#define NAMEDPIPE 2
#define SOUND 3
#define MAX_MESSAGE_SIZE 256

void start();
int dialog();
void echoServerHandle();

#endif //CLIENT_H
