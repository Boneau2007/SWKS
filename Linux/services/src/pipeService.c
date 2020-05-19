#include "pipeService.h"

/*
 * @function	init
 * @abstract	Accepts a client connection
 * @discuss 	This function accepts the client connection and passes it from the
 * 				listing socket to another socket.
 * @param		listener	Pointer-Id of the listing-socket
 * @param		worker		Pointer-Id of the working-socket
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int init(const char* path, mode_t mode){
	if(mkfifo(path, mode) == -1){
		printf("An error has occured\n");
		switch(errno){
			case EACCES : printf("No execute permission on path\n");
						  break;
			case EDQUOT : printf("Exhausted quota of inodes or disk blocks\n");
						  break;
			case EEXIST : printf("Pathname already exists\n");
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

/*
 * @function	readHandle
 * @abstract	Accepts a client connection
 * @discuss 	This function accepts the client connection and passes it from the
 * 				listing socket to another socket.
 * @param		listener	Pointer-Id of the file-descriptor
 * @param		path		Path of the Named-Pipe
 */
void readHandle(int* reader, const char* path) {
	char * buffer = calloc (MAX_BUFF_SIZE, sizeof(char));
	read(*reader, buffer, MAX_BUFF_SIZE);
	if(strlen(buffer) > 0){
		printf("##########\n");
		printf("Pipe-Input:\n%s",buffer);
		printf("##########\n");
		close(*reader);
		*reader = open(path, O_RDONLY | O_NONBLOCK);
	}
	free(buffer);
}

/*
 * @function	writeHandle
 * @abstract	Accepts a client connection
 * @discuss 	This function accepts the client connection and passes it from the
 * 				listing socket to another socket.
 * @param		listener	Pointer-Id of the file-descriptor
 * @param		path		Path of the Named-Pipe
 */
void writeHandle(int worker, int *reader, const char* path, const char* message, int length) {
	char* buffer = calloc(MAX_BUFF_SIZE ,sizeof(char));
	char pidBuff[16];
	sprintf(pidBuff,"Server with Pid [%d]: ",getpid());
	strncpy(buffer, pidBuff, strlen(pidBuff));
	strncat(buffer, message, length);
	close(*reader);
	worker = open(path, O_WRONLY);
	write(worker, buffer, strlen(buffer));
	fflush(stdout);
	close(worker);
	*reader = open(path, O_RDONLY | O_NONBLOCK);
	free(buffer);
}

