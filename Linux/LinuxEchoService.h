#ifndef LINUXECHOSERVICE_H
#define LINUXECHOSERVICE_H
#define MAX_WORKER 20

int start();
int pause();
int resume();
void concatString(char* source, char* add);

#endif //LINUXECHOSERVICE_H
