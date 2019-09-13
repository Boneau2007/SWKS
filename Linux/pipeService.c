#include "pipeService.h"
void initNamedPipe(char* path, mode_t mode){
	if(mkfifo(path, mode) == -1){
		printf("Couldn't create fifo\n");
	}
}
void handleNamedPipeService() {
}