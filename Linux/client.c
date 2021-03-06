#include "client.h"

int init = -1;
unsigned int samples = 44100;
char* deviceName = "default";
int channels = 1;
snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;
snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
snd_pcm_access_t mode = SND_PCM_ACCESS_RW_INTERLEAVED;
snd_pcm_uframes_t frames = 2;
snd_pcm_t* pcmHandle;

void echoServiceHandle(){
  struct sockaddr_in server;
  char* clientMessage = calloc(MAX_MESSAGE_SIZE, sizeof(char));
  char* serverMessage = calloc(MAX_MESSAGE_SIZE, sizeof(char));


  struct timeval time;
  time.tv_sec = 10;
  time.tv_usec = 0;

  int socket = createSocket(AF_INET, SOCK_STREAM, 0);
  setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&time, sizeof(time));
  setServerAddress(&server);

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
      
      if(strncmp(clientMessage, "CLOSE", 5) == 0){
        printf("Close connection..\n");
        break;
      }

      write(socket, clientMessage, strlen(clientMessage));
      if(recv(socket, serverMessage, MAX_MESSAGE_SIZE, 0) < 0){
        printf("Server response timeout\n");
        break;
      }else{
        printf("%s\n", serverMessage);
      }
    }
  }else{
      printf("\nCould not establish connection to the server.\n");
  }
  closeSocket(&socket);
}

void soundServiceHandle(){  
  struct sockaddr_in serverAddress;
  setServerAddress(&serverAddress);
  int socket = createSocket(AF_INET, SOCK_DGRAM, 0);
  long size = (frames * snd_pcm_format_width(format)) / 8 * 2;
  char* message = calloc(size, sizeof(char)); // bytes/sample, channels
  time_t count = getInt("\nHow many seconds do you want to Speak : ");
  time_t start = time(NULL);
  do{
    if(snd_pcm_readi(pcmHandle, message, size) < 0){
      snd_pcm_prepare(pcmHandle);
      printf("##### buffer underrun #####");
    }else{
      sendto(socket, message, size, MSG_WAITALL, (const struct sockaddr*)&serverAddress, sizeof(serverAddress));
    }
  }while( time(NULL)-start < count);
  snd_pcm_close (pcmHandle);
}

void setServerAddress(struct sockaddr_in* server){
  struct hostent* host_info;
  unsigned long address;
  char* host = calloc(MAX_MESSAGE_SIZE, sizeof(char));

  printf("\nPlease insert IP or Host name: ");
  fgets(host, 32, stdin);
  fflush(stdin);
  if((address = inet_addr(host))!= INADDR_NONE){
    memcpy((char *)&server->sin_addr, &address, sizeof(address));
  }else{
    host_info = gethostbyname(host);
    if(host_info==NULL){
      printf("\nUnknown Server");
      return;
    }else{
      memcpy((char *)&server->sin_addr, &host_info->h_name, host_info->h_length);
    }
  }
  int port = getInt("\nPlease insert Port: ");
  server->sin_family = AF_INET;
  server->sin_port = htons(port);
}

int getInt(const char* message){
  printf("%s",message);
  long value;
  fflush(stdin);
  fgets((char*)&value, sizeof(value)-1, stdin);
  fflush(stdin);
  value = atoi((char*)&value);
  return value;
}

int dialog(){
  printf("\n==========================");
  printf("\n%i. Connect to Echo-Server", ECHOSERVER);
  printf("\n%i. Speak to Server", SOUND);
  printf("\n%i. Close Client", END);
  printf("\n==========================");
  int input = getInt("\nEnter Value: ");
  if(input >= 0 && input <= 9){
    return input;
  }
  return -1;
}

void start(){
  while(init != END){
  init = dialog();
  //init = 3; // Nur zum test
  switch (init) {
    case ECHOSERVER: printf("\nStart connection to Echo-Server now..\n");
                     echoServiceHandle();
                     break;
		  
    case SOUND:      printf("\nStart Sound connection..\n");
                     if(initSoundDevice(&pcmHandle,deviceName,channels,&samples, stream, format, mode, frames)==EXIT_FAILURE){
                       printf("Could not set PCM-Device");
                     }else{
                       soundServiceHandle();
                     }
                     break;
		  
    case END:        printf("\nClient geschlossen\n");
                     break;
		  
    default:         printf("\nWrong input, must between 0 and 3");
                     break;
    }
  }
}

int main(int argc, char** argv){
  if(argc > 1){
    printf("usage: ./client\n");
    return EXIT_FAILURE;
  }else{
    start();
  }
  return EXIT_SUCCESS;
}
