#ifndef ECHOSERVICE_H
#define ECHOSERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "../../include/linuxsocket.h"

#define BUFF_SIZE 256

extern int init(int* listener, int* worker, const char* message);
extern int handle(int* socket);
extern int close(int* socket);

#endif //ECHOSERVICE_H
