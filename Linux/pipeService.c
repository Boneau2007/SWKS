#include "pipeService.h"

int initNamedPipe(const char* path, mode_t mode){
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

void openPipeService(int* reader, const char* path){
	*reader = open(path, O_RDONLY | O_NONBLOCK);
}

void handleNamedPipeServiceWrite(int* worker) {
	char buffer[MAX_BUFF_SIZE];
	int pid = getpid();    
	sprintf(buffer, "%d: ", pid);
	fgets((char*)&buffer, MAX_BUFF_SIZE-sizeof(pid), stdin);
	fflush(stdin);
	write(*worker, buffer, MAX_BUFF_SIZE);
}
void handleNamedPipeServiceRead(int* worker) {
	char buffer[MAX_BUFF_SIZE];
	read(*worker, buffer, MAX_BUFF_SIZE);
	printf("Pipe-Input: %s\n",buffer);
}

void closeNamedPipeService(int* worker){
	close(*worker);
}