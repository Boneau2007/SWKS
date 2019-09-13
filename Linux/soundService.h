#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>    
#include <errno.h>
#include <alsa/asoundlib.h>
#include "linuxsocket.h"
#include "echoService.h"

extern void initSoundDevice(const char* name, int channels, unsigned int* sampleRate);
extern void handleSoundServiceConnect(int* listenerSocket, int* workerSocket);
extern void handleSoundService();

#endif //SOUNDSERVICE_H
