#include <stdlib.h>
#include <string.h>
#include "registry.h"

int main(int argc, char** argv) {
  //Starte alle in einem eigenen Thread
  initializeServer(8001,8002);
  startConnectionHandle();
  //handleNamedPipeService("/tmp/fifio", O_RDWR);
}
