#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "linuxsocket.h"

int main(int argc, char** argv){
    int tcpListener = createSocket(AF_INET, SOCK_STREAM, 0);
  	bindSocket(tcpListener, INADDR_ANY, 8001);
  	listenSocket(tcpListener);
    while(1){}
}
