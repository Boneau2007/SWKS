#include "linuxsocket.h"
#include "client.h"

int init = -1;

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
      case SOUND: printf("\nStart game..");
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
  printf("\n%i. Connect to Game", SOUND);
  printf("\n%i. Close Client", END);
	printf("\n==========================");
  printf("\nEnter Value: ");
  fgets((char*)&input, 16, stdin);
  fflush(stdin);
  input = atoi((char*)&input);
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
  clientMessage = calloc(MAX_MESSAGE_SIZE, sizeof(char));
  serverMessage = calloc(MAX_MESSAGE_SIZE, sizeof(char));
  host = calloc(MAX_MESSAGE_SIZE, sizeof(char));

  struct timeval time;
  time.tv_sec = 10;
  time.tv_usec = 0;

  printf("\nPlease insert IP or Host name: ");
  fgets(host, 32, stdin);
  fflush(stdin);
  if((address = inet_addr(host))!= INADDR_NONE){
    memcpy((char *)&server.sin_addr, &address, sizeof(address));
  }else{
    host_info = gethostbyname(host);
    if(host_info==NULL){
      printf("\nUnknown Server");
      return;
    }else{
      memcpy((char *)&server.sin_addr, &host_info->h_name, host_info->h_length);
    }
  }

  printf("\nPlease insert Port: ");
  fgets((char*)&port, 16, stdin);
  fflush(stdin);
  port = atoi((char*)&port);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  int socket = createSocket(AF_INET, SOCK_STREAM, 0);
  setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&time, sizeof(time));
  
  if(connect(socket, (struct sockaddr*)&server, sizeof(server)) != -1){
      recv(socket, serverMessage, MAX_MESSAGE_SIZE, 0);
      printf("%s", serverMessage);
    while(1){
      //Re-/Set messagebuffers to \0
      memset(serverMessage,'\0',MAX_MESSAGE_SIZE*sizeof(char));
      memset(clientMessage,'\0',MAX_MESSAGE_SIZE*sizeof(char));
      
      //Get Message
      printf("\nPlease insert a Message or [CLOSE] for close: ");
      fgets(clientMessage, MAX_MESSAGE_SIZE, stdin);
      fflush(stdin);
      int len = strlen(clientMessage);
      
      if(strncmp(clientMessage, "CLOSE", 5) == 0){
        printf("Close connection..\n");
        break;
      }

      write(socket, clientMessage, len);
      if(recv(socket, serverMessage, MAX_MESSAGE_SIZE, 0)<0){
        printf("Server response timeout\n");
        break;
      }else{
        printf("%s", serverMessage);
      }
    }
  }else{
      printf("\nCould not establish connection to the server.\n");
  }
  closeSocket(&socket);
}
