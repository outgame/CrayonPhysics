#Sam's Makefile for physics.c and server stuff

COMPILEFLAGS = -std=c99 -Wall -pedantic -g -DDEBUG
CFLAGS = $(COMPILEFLAGS)
CFLAGS += `pkg-config --cflags gtk+-3.0`
#CFLAGS += -I/usr/local/include

LIBPATH = -L/usr/local/include/chipmunk
LIBRARIES = -lchipmunk -lm `pkg-config --libs gtk+-3.0`

FILENAME = physics

all: server client Makefile

physics.o: $(FILENAME).c $(FILENAME).h
	gcc -c $(CFLAGS) -o $(FILENAME).o $(FILENAME).c

phys:  $(FILENAME).o
	gcc -o phys $(LIBPATH) $(LIBRARIES) $(FILENAME).o

server: server.c server.h
	gcc $(CFLAGS) -o server server.c

client: client.c client.h
	gcc $(CFLAGS) -o client client.c

clean:
	rm -rf *~ *.o phys server client *.dSYM
	ls -G