#include <stdlib.h>
#include <string.h>
#include "registry.h"

int main(int argc, char** argv) {
  initializeServer(8001,8002);
  startConnectionHandle();
}
