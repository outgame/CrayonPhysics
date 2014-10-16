PHYS_PATH = ../core/physics
GRAPH_PATH = ../graphics/graphics
LEVEL_PATH = ../core/level
SERVER_PATH = ../core/server
CLIENT_PATH = ../core/client

CC = gcc
CFLAGS = -Wall -g -std="c99" -pedantic -I/usr/local/include/chipmunk `pkg-config --cflags gtk+-3.0`

LIBRARIES = -lm -L/usr/local/lib -lchipmunk `pkg-config --libs gtk+-3.0` -L/usr/lib64/ -lpthread


OBJS = physics.o gui.o graphics.o level.o client.o

all: exp  gui Makefile

exp: 
	export LD_LIBRARY_PATH=/usr/local/lib/


gui: $(OBJS)
	$(CC) -o gui $(LIBRARIES) $(OBJS)

level.o: $(LEVEL_PATH).c $(LEVEL_PATH).h
	$(CC) -c $(CFLAGS) -o level.o $(LEVEL_PATH).c

physics.o: $(PHYS_PATH).c ../core/physics.h
	$(CC) -c $(CFLAGS) -o physics.o $(PHYS_PATH).c

gui.o: gui.c gui.h
	$(CC) -c $(CFLAGS) -o gui.o gui.c $(LIBRARIES)

graphics.o: $(GRAPH_PATH).c $(GRAPH_PATH).h
	$(CC) -c $(CFLAGS) -o graphics.o $(GRAPH_PATH).c

client.o: $(CLIENT_PATH).c $(CLIENT_PATH).h
	$(CC) -c $(CFLAGS) -o client.o $(CLIENT_PATH).c

clean:
	rm -f gui *.o *.dSYM *~
