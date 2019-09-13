# Makefile für main

CC = gcc
CFLAGS = -Wall -pedantic
OBJ = *.o

all: server client clean

client: client.o linuxsocket.o
	${CC} ${CFLAGS} -o client client.o linuxsocket.o

server: main.o linuxsocket.o services.o
	${CC} ${CFLAGS} -o main main.o linuxsocket.o services.o

linuxsocket.o:
	${CC} ${CFLAGS} -c linuxsocket.c

services.o:
	${CC} ${CFLAGS} -c services.c

client.o:
	${CC} ${CFLAGS} -c client.c

main.o:
		${CC} ${CFLAGS} -c main.c

# Bereinigung des Verzeichnisses
clean:
	@ echo "\nLösche dateien mit Endung ${OBJ}"
	rm -f ${OBJ}
