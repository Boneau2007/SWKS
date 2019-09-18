#include <stdlib.h>
#include <string.h>
#include "registry.h"

int main(int argc, char** argv) {
  int tcpSocket;
  int udpSocket;
  const char* path = "/tmp/fifo1";
  if((tcpSocket = initTcpSocket(8001)) < 0){
    exit(EXIT_FAILURE);
  }  
  if((udpSocket = initUdpSocket(8002)) < 0){
    exit(EXIT_FAILURE);
  }
  if((initPipe(path,0666))< 0){
    exit(EXIT_FAILURE);
  }
  if(initSound()){
    exit(EXIT_FAILURE);
  }
  printf("\nServer-configuration succeeded\n");
  startConnectionHandle(&tcpSocket, &udpSocket, path);
}
