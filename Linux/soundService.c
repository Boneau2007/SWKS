#include "soundService.h"

/*
 * @function	handleSoundService
 * @abstract	Handles incomming communication
 * @discuss 	This function handles the incomming sound communication
 * @param		  int         socket-Id   
 * @param		  snd_pcm_t		pmHandle
 */
int handleSoundService(int socket, snd_pcm_t* pcmHandle) {
  int length = 16;
  char * data = calloc(length ,sizeof(char));
  if((length = read(socket, data, length)) >= 0){ 
    while ((snd_pcm_writei(pcmHandle, data, length)) < 0) {
      snd_pcm_prepare(pcmHandle);
      fprintf(stderr,"<##### buffer underrun #####>\n");
    }
    free(data);
    return -1;
  }else{
    free(data);
    return EXIT_FAILURE;
  }
}

/*
 * @function	initSoundDevice
 * @abstract	Initializes the device
 * @discuss 	This function is responsible for the device initialization
 * @param		  deviceName		      Name of the PCM-device
 * @param		  channels		        Channel count
 * @param		  sampleRate		      SampleRate of the device
 * @param		  snd_pcm_format_t		see alsa/asounldlib.h
 * @param		  snd_pcm_stream_t		see alsa/asounldlib.h
 * @param		  snd_pcm_access_t		see alsa/asounldlib.h
 * @result		Results an pointer of pcmHandle or NULL
 */
int initSoundDevice(snd_pcm_t** handle, const char* deviceName, int channels, unsigned int * sampleRate,
                           snd_pcm_stream_t stream,
                           snd_pcm_format_t format, snd_pcm_access_t mode,
                           snd_pcm_uframes_t frames){
  
  snd_pcm_hw_params_t* params;
  unsigned int pcmFd; 

  snd_pcm_hw_params_alloca(&params);

 //Creates a handle and opens a specific audio interface
  if((pcmFd = snd_pcm_open(handle, deviceName, stream,0)) < 0){
    fprintf(stderr,"Error: Couldnt open default PCM device : [%d]", pcmFd);
    return EXIT_FAILURE;
  }
  fprintf(stdout,"PCM device opened\n");

  if (snd_pcm_hw_params_malloc(&params) < 0) {
    fprintf(stderr,"Error: Could not allocate HW parameter for PCM device\n");
    return EXIT_FAILURE;
  }
  fprintf(stdout,"Allocated hw params for device\n");

	//configure std params to the device
  if(snd_pcm_hw_params_any(*handle, params)< 0){
    fprintf(stderr,"Error: Could not configure PCM device\n");
    return EXIT_FAILURE;
  }
  fprintf(stdout,"Set default params\n");

	//set the device in interleaved mode
  if (snd_pcm_hw_params_set_access(*handle, params, mode) < 0) {
	  fprintf(stderr,"Error: Could not set PCM-device mode\n");
    return EXIT_FAILURE;
  }
  fprintf(stdout,"Set access mode\n");

	//set sample format to the device
	if (snd_pcm_hw_params_set_format(*handle, params,	format) < 0) {
	  fprintf(stderr,"Error: Could not set sample format\n");
    return EXIT_FAILURE;
  }
  fprintf(stdout,"Sample format set\n");
	
  //set channcel size to the device
	if (snd_pcm_hw_params_set_channels(*handle, params, channels) < 0) {
		fprintf(stderr,"Error: Could not set Channels\n");
    return EXIT_FAILURE;
  }
  fprintf(stdout,"Channel size set : [%d]\n",channels);
	
  //set sample rate of the device, 44100(CD Quality)
	if (snd_pcm_hw_params_set_rate_near(*handle, params, sampleRate, 0) < 0) {
		fprintf(stderr,"Error: Could not set sample rate\n");
    return EXIT_FAILURE;
  }
	fprintf(stdout,"Sample Rate set : [%d Hz]\n",*sampleRate);
	
  //Set period size
	if (snd_pcm_hw_params_set_period_size_near(*handle, params, &frames, 0) < 0) {
		fprintf(stderr,"Error: Could not set period size\n");
    return EXIT_FAILURE;
  }
	fprintf(stdout,"Period size set : [%d]\n", (int)frames);
	
	//Write hardware-parameter to device
	if(snd_pcm_hw_params(*handle, params)<0){
		fprintf(stderr,"Error: Could not apply hardware parameters\n");
    return EXIT_FAILURE;
	}
  fprintf(stdout,"PCM device prepared and configured.");
  snd_pcm_hw_params_free(params);
    return EXIT_SUCCESS;
  
}

void closeSoundService(snd_pcm_t* pcmHandle) {

  // Drop pending frames
  snd_pcm_drop(pcmHandle);

  // Stop PCM device after pending frames have been played 
  snd_pcm_drain(pcmHandle);

  // Close PCM device after drained 
  snd_pcm_close(pcmHandle);

  //free data buffer
}
