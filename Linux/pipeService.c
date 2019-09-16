#include "pipeService.h"

char inputbuffer[MAX_BUFF_SIZE];
char outputbuffer[MAX_BUFF_SIZE];

int initNamedPipe(char* path, mode_t mode){
	if(mkfifo(path, mode) == -1){
		printf("An error has occured\n");
		switch(errno){
			case EACCES : printf("No execute permission on path\n");
						  break;
			case EDQUOT : printf("Exhausted quota of inodes or disk blocks\n");
						  break;
			case EEXIST : printf("Pathname already exists\n");
						  return EEXIST;
						  break;
			case ENAMETOOLONG : printf("Name to long\n");
						  break;
			case ENOENT : printf("Not existing path\n");
						  break;
			case ENOSPC : printf("Not enough space\n");
						  break;
			case ENOTDIR : printf("Artefact in pathname was not one\n");
						  break;
			case EROFS : printf("Path is readonly\n");
						  break;
			default : printf("Could not create FIFO\n");
		}
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void connectToPipeService(int* worker,char* path, int mode){
	*worker = open(path, mode);

}

void handleNamedPipeService(int* worker) {
	read(worker, inputbuffer, MAX_BUFF_SIZE);
}

void closeNamedPipeService(int* worker){
	close(*worker);
}