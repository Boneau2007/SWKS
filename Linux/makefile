# Makefile für main

CC = gcc
G++ = g++
CFLAGS = -Wall -pedantic
OBJ = *.o

all: client server serverT *.o

client: client.o linuxsocket.o soundService.o 
	${CC} ${CFLAGS} -o client client.o linuxsocket.o soundService.o -lasound

server: main.o linuxsocket.o registry.o echoService.o pipeService.o soundService.o 
	${CC} ${CFLAGS} -o server main.o linuxsocket.o registry.o echoService.o pipeService.o soundService.o -lasound 

serverT: mainT.o linuxsocket.o registryT.o echoService.o pipeService.o soundService.o 
	${G++} ${CFLAGS} -o serverT main.o linuxsocket.o registry.o echoService.o pipeService.o soundService.o -lasound -pthread

linuxsocket.o:
	${CC} ${CFLAGS} -c linuxsocket.c

registry.o:
	${CC} ${CFLAGS} -c registry.c

registryT.o:
	${G++} ${CFLAGS} -std=c++11 -c registry.cpp

echoService.o:
	${CC} ${CFLAGS} -c echoService.c

pipeService.o:
	${CC} ${CFLAGS} -c pipeService.c

soundService.o:
	${CC} ${CFLAGS} -c soundService.c 

client.o:
	${CC} ${CFLAGS} -c client.c

main.o:
	${CC} ${CFLAGS} -c main.c

mainT.o:
	${G++} ${CFLAGS} -std=c++11 -c main.c

# Bereinigung des Verzeichnisses
*.o:
	@ echo "\nLösche dateien mit Endung ${OBJ}"
	rm -f ${OBJ}
 clean:
	@ echo "\nLösche alle Artefakte"
	rm -f ${OBJ} server serverT client