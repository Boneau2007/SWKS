#include <stdlib.h>
#include <string.h>
#include "registry.h"

int main(int argc, char** argv) {
    int tcpSocket, udpSocket;

    if(argc != 5){
        printf("usage: ./server [tcpPort] [udpPort] [pipePath] [permission]\n");
        exit(EXIT_FAILURE);
    }

    const int tcpPort = atoi(argv[1]);
    const int udpPort = atoi(argv[2]);
    const char* path = argv[3];
    const int permission = atoi(argv[4]);

    if( (tcpSocket = initTcpSocket(tcpPort)) < 0 || (udpSocket = initUdpSocket(udpPort)) < 0 ||
        (initPipe(path,permission)) < 0 || initSound()){
        exit(EXIT_FAILURE);
    }
    printf("\nServer-configuration succeeded\n");
    startConnectionHandle(&tcpSocket, &udpSocket, path);
}
