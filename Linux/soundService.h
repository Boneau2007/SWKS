#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H

#include <stdio.h>
#include <alsa/asoundlib.h>
#include "linuxsocket.h"

extern int initSoundDevice(snd_pcm_t** handle, const char* deviceName, int channels, unsigned int * sampleRate,snd_pcm_stream_t stream,
                                  snd_pcm_format_t format, snd_pcm_access_t mode,
                                  snd_pcm_uframes_t frames);
extern int handleSoundService(int socket, snd_pcm_t* pcmHandle);
extern void closeSoundService(snd_pcm_t* pcmHandle);

#endif //SOUNDSERVICE_H
