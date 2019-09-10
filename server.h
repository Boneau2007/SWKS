#ifndef Server_H
#define Server_H

#define MAX_WORKER 20
#define BUFFER_SIZE 256

extern int start(int port);
extern int pause();
extern int stop();
extern int resume();
extern int chooseService(char*);
extern int chooseAction(char*);
extern char* currentState();
extern int performStateSwitch();
extern void concatString(char*, char*);

#endif // Server_H
