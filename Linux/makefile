# Makefile für main

CC = gcc
CFLAGS = -Wall -pedantic
OBJ = *.o

all: server client clean

client: client.o linuxsocket.o
	${CC} ${CFLAGS} -o client client.o linuxsocket.o

server: main.o linuxsocket.o registry.o echoService.o pipeService.o soundService.o
	${CC} ${CFLAGS} -o main main.o linuxsocket.o registry.o echoService.o pipeService.o soundService.o -lasound

linuxsocket.o:
	${CC} ${CFLAGS} -c linuxsocket.c

registry.o:
	${CC} ${CFLAGS} -c registry.c

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

# Bereinigung des Verzeichnisses
clean:
	@ echo "\nLösche dateien mit Endung ${OBJ}"
	rm -f ${OBJ}
