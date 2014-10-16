#Sam's Makefile for physics.c and server stuff

COMPILEFLAGS = -std=c99 -Wall -pedantic -g #-DDEBUG
CFLAGS = $(COMPILEFLAGS)
CFLAGS += `pkg-config --cflags gtk+-3.0`
#CFLAGS += -I/usr/local/include

LIBPATH = -L/usr/local/include/chipmunk
LIBRARIES = -lchipmunk -lm `pkg-config --libs gtk+-3.0`

FILENAME = physics

all: server Makefile

physics.o: $(FILENAME).c $(FILENAME).h
	gcc -c $(CFLAGS) -DDEBUG -o $(FILENAME).o $(FILENAME).c

phys:  $(FILENAME).o
	gcc -o phys $(LIBPATH) $(LIBRARIES) $(FILENAME).o

server: server.c
	gcc $(COMPILEFLAGS) -D_POSIX_C_SOURCE=200112L -o server server.c

client: client.c client.h
	gcc $(COMPILEFLAGS) -DDEBUG -o client client.c

clean:
	rm -rf *~ *.o phys server client a.out *.dSYM
	ls -G
