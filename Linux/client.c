#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "linuxsocket.h"

#define END  0
#define ECHOSERVER 1
#define NAMEDPIPE 2
#define GAME 3

int init = -1;

void start();
int dialog();
void echoServerHandle();

int main(int argc, char** argv){
  if(argc > 1){
    printf("usage: ./client\n");
    return EXIT_FAILURE;
  }else{
    start();
  }
  return EXIT_SUCCESS;
}

void start(){
  while(init != END){
  init = dialog();
    switch (init) {
      case ECHOSERVER: printf("\nStart connection to Echo-Server now..\n");
                       echoServerHandle();
                       break;
      case NAMEDPIPE: printf("\nBegin with pipe");
                      break;
      case GAME: printf("\nStart game..");
                 break;
      case END: printf("\nClient geschlossen\n");
                 break;
      default: printf("\nWrong input, must between 0 and 3");

               break;
    }
  }
}


int dialog(){
  int input;
	printf("\n==========================");
  printf("\n%i. Connect to Echo-Server", ECHOSERVER);
  printf("\n%i. Process through Named-pipe", NAMEDPIPE);
  printf("\n%i. Connect to Game", GAME);
  printf("\n%i. Close Client", END);
	printf("\n==========================");
  printf("\nEnter Value: ");
  scanf("%d", &input);
  while (getchar() != '\n' );
  printf("Input was: %x",input);
  if(input >= 0 && input <= 9){
    return input;
  }
  return -1;
}

void echoServerHandle(){
  char* clientMessage;
  char* serverMessage;
  char* host;
  int port;
  struct sockaddr_in server;
  struct hostent* host_info;
  unsigned long address;
  memset(&server,0,sizeof(server));
  clientMessage = calloc(256, sizeof(char));
  serverMessage = calloc(256, sizeof(char));
  host = calloc(256, sizeof(char));


  printf("\nPlease insert IP or Host name: ");
  scanf("%s",host);
  while (getchar() != '\n' );
  if((address = inet_addr(host))!= INADDR_NONE){
    memcpy((char *)&server.sin_addr, &address, sizeof(address));
  }else{
    host_info = gethostbyname(host);
    if(host_info==NULL){
      printf("\nUnknown Server");
      return;
    }else{
      memcpy((char *)&server.sin_addr, &host_info->h_addr, host_info->h_length);
    }
  }

  printf("\nPlease insert Port: ");
  scanf("%d", &port);
  while (getchar() != '\n' );
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  int socket = createSocket(AF_INET, SOCK_STREAM, 0);
  if(connect(socket, (struct sockaddr*)&server, sizeof(server)) != -1){
    //recv(socket, &serverMessage, 256, 0);
    while(strcmp(clientMessage, "END")){
      memset(clientMessage,0,256*sizeof(char));
      scanf("%s", clientMessage);
      while (getchar() != '\n' );
      send(socket, clientMessage, 256, 0);

      memset(serverMessage,0,256*sizeof(char));
      recv(socket, serverMessage, 256, 0);
      printf("%s", serverMessage);
    }
  }else{
      printf("\nCould not establish connection to the server.\n");
  }
  closeSocket(&socket);
}
