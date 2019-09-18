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

void handleNamedPipeServiceWrite(int worker,int reader, const char* path) {
	char buffer[MAX_BUFF_SIZE];		
	char message[MAX_BUFF_SIZE];	
	char pidBuff[16];
	close(reader);
	worker = open(path, O_WRONLY);
	sprintf(pidBuff,"Server with Pid [%d]: ",getpid());
	fgets((char*)&buffer, MAX_BUFF_SIZE, stdin);
	fflush(stdin);
	strncpy(message, pidBuff, strlen(pidBuff));
	strncat(message, buffer, strlen(buffer));
	write(worker, message, strlen(message));
	close(worker);
	openPipeService(&reader, path);
}
void handleNamedPipeServiceRead(int worker, const char* path) {
	char * buffer = calloc (MAX_BUFF_SIZE, sizeof(char));
	read(worker, buffer, MAX_BUFF_SIZE);
	printf("##########\n");
	printf("Pipe-Input:\n%s",buffer);
	printf("##########\n");
	close(worker);
	openPipeService(&worker, path);
}

void closeNamedPipeService(int* worker){
	close(*worker);
}