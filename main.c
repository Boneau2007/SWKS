#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "Server.h"


typedef enum {start, stop, pause, resume} MODE;

pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lockState = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockMode = PTHREAD_MUTEX_INITIALIZER;

int state = start;
int mode = start;

void* serverSig(void*);
void* watchSignal(void*);

int main(int argc, char** argv) {
	if(arc != 2){
		printf("usage: Server\n");
		return EXIT_FAILURE;
	}

	pthread_t tid1, tid2;

	//Create Thread 1
	pthread_create(&tid1,NULL,serverSig,NULL);

	sleep(1);
	//Create Thread 2
	pthread_create(&tid2,NULL,serverExcec,NULL);

	//Wait for the completion of the Thread 2
	pthread_join(tid2,NULL);
	return EXIT_SUCCESS;
}
void* serverSig(void* args){
	do{
			printf("[s]top\t[p]ause\t[r]esume : ");
			char c = getchar( );
			printf( "\nYou entered: ");
			putchar( c );
			pthread_mutex_lock(&lockState);
			if(c=='s'){
				printf("\n Server will now stop");
					state = stop;
					exit(EXIT_SUCCESS);
			}else if(c=='p'){
				if(state == paused){
					printf("\n Server is already paused.");
				}else{
				printf("\n Server will now pause");
					state = pause;
				}
			}else if(c=='r'){
				if(state == resume){
					printf("\n Server is already running.");
				}else{
					printf("\n Server will now resume");
						state = resume;
				}
			}else{
				printf("\n Command not recognized");
				state = mode;
			}
		pthread_mutex_unlock(&lockState);
		sleep(1);
		}while(c != 's');
		pthread_exit(NULL);
}

void* serverExcec(void*){
	pthread_mutex_lock(&lockMode);
	pthread_mutex_lock(&lockState);
	// critical section
	if(state != mode){
		mode = state;
	pthread_mutex_unlock(&lockMode);
	pthread_mutex_unlock(&lockState);
		if(mode = pause){

		} else if(mode = resume){
			resume()
		}
	}
pthread_exit(NULL);
}
