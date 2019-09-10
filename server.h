#ifndef Server_H
#define Server_H

#define MAX_WORKER 20
#define BUFFER_SIZE 256

int start(int port);
int pause();
int stop();
int resume();
extern void concatString(char*, char*);

#endif // Server_H
