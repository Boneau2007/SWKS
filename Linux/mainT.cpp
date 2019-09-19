#include <stdlib.h>
#include <string.h>
#include "registryT.hpp"

int main(int argc, char** argv) {
  if(argc != 5){
    printf("usage: ./server [TcpPort] [UdpPort] [PipePath] [Permission]\n");
    exit(EXIT_FAILURE);
  }
  int tcpSocket;
  int tcpPort = atoi(argv[1]);
  
  int udpSocket;
  int udpPort = atoi(argv[2]);

  const char* path = argv[3];
  int permission = atoi(argv[4]);

  if((tcpSocket = initTcpSocket(tcpPort)) < 0){
    exit(EXIT_FAILURE);
  }  
  if((udpSocket = initUdpSocket(udpPort)) < 0){
    exit(EXIT_FAILURE);
  }
  if((initPipe(path,permission))< 0){
    exit(EXIT_FAILURE);
  }
  if(initSound()){
    exit(EXIT_FAILURE);
  }
  printf("\nServer-configuration succeeded\n");
  startConnectionHandle(&tcpSocket, &udpSocket, path);
}
