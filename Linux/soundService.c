#include "soundService.h"

#include <alsa/asoundlib.h>
/* Name of the PCM device, like plughw:0,0          */
/* The first number is the number of the soundcard, */
/* the second number is the number of the device.   */
void initSoundDevice(const char* name, int channels, unsigned int * sampleRate){

snd_pcm_t* pcmHandle;
unsigned int pcmFd; 

/* This structure contains information about    */
/* the hardware and can be used to specify the  */      
/* configuration to be used for the PCM stream. */ 
snd_pcm_hw_params_t* params;

snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;

    snd_pcm_hw_params_alloca(&params);

 //Creates a handle and opens a specific audio interface
    if((pcmFd = snd_pcm_open(&pcmHandle, "default", stream,0)) < 0){
        printf("Couldnt open default PCM device : [%d]", pcmFd);
    }
    

	//configure std params to the device
    if(snd_pcm_hw_params_any(pcmHandle, params)< 0){
        printf("Could not configure PCM device\n");
    }

	//set the device in interleaved mode
    if (snd_pcm_hw_params_set_access(pcmHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
		printf("PCM device could not be set to interleaved\n");

	//set sample format to the device
	if (snd_pcm_hw_params_set_format(pcmHandle, params,	SND_PCM_FORMAT_S16_LE) < 0) 
		printf("Could not set sample format\n");

	//set channcel size to the device
	if (snd_pcm_hw_params_set_channels(pcmHandle, params, channels) < 0) 
		printf("Could not set Channels\n");

	//set sample rate of the device
	if (snd_pcm_hw_params_set_rate_near(pcmHandle, params, sampleRate, 0) < 0) 
		printf("Could not set sample rate\n");
	
	//Write hardware-parameter to device
	if(snd_pcm_hw_params(pcmHandle, params)<0){
		printf("Could not apply hardware parameters\n");
	}


int num_frames;
/* Write num_frames frames from buffer data to    */ 
/* the PCM device pointed to by pcm_handle.       */
/* Returns the number of frames actually written. */
snd_pcm_sframes_t snd_pcm_writei(pcm_handle, data, num_frames);
  
/* Write num_frames frames from buffer data to    */ 
/* the PCM device pointed to by pcm_handle.       */ 
/* Returns the number of frames actually written. */
snd_pcm_sframes_t snd_pcm_writen(pcm_handle, data, num_frames);
  
snd_pcm_uframes_t periodsize = 8192; /* Periodsize (bytes) */

unsigned char *data;
int pcmreturn, l1, l2;
short s1, s2;
int frames;

data = (unsigned char *)malloc(periodsize);
frames = periodsize >> 2;
for(l1 = 0; l1 < 100; l1++) {
  for(l2 = 0; l2 < num_frames; l2++) {
    s1 = (l2 % 128) * 100 - 5000;  
    s2 = (l2 % 256) * 100 - 5000;  
    data[4*l2] = (unsigned char)s1;
    data[4*l2+1] = s1 >> 8;
    data[4*l2+2] = (unsigned char)s2;
    data[4*l2+3] = s2 >> 8;
  }
  while ((pcmreturn = snd_pcm_writei(pcmHandle, data, frames)) < 0) {
    snd_pcm_prepare(pcmHandle);
    fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
  }
}
  
/* Stop PCM device and drop pending frames */
snd_pcm_drop(pcmHandle);

/* Stop PCM device after pending frames have been played */ 
snd_pcm_drain(pcmHandle);
}

void handleSoundService() {
}

void handleSoundServiceConnect(int* listenerSocket, int* workerSocket) {
}
