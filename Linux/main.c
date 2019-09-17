#include <stdlib.h>
#include <string.h>
#include "registry.h"

int main(int argc, char** argv) {
  if(initializeServer(8001,8002, "/tmp/fifo0",0666)){
    exit(EXIT_FAILURE);
  }
  printf("\nServer-configuration succeeded\n");
  startConnectionHandle();
}
