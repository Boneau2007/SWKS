# Makefile für main

CC = gcc
CFLAGS = -Wall -pedantic
OBJ = *.o

all: main.o sockets.o server.o
	${CC} ${CFLAGS} -o $@ sockets.o server.o

sockets.o: sockets.c sockets.h
	${CC} ${CFLAGS} -c sockets.c

server.o: server.c server.h
		${CC} ${CFLAGS} -c server.c

# Bereinigung des Verzeichnisses
clean:
@echo "Folgende Dateien werden geloescht"
@ ${OBJ}
rm -f ${OBJ}
