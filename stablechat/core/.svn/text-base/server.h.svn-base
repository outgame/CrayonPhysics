#ifndef SERVER_HEADER
#define SERVER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define TRUE 1
#define ERROR -1

#define LISTENQ 8
#define MAXLINE 256

#define TIMEOUT_SEC 100
#define TIMEOUT_USEC 0

int server_start();
void *get_in_addr(struct sockaddr *sa);

int server_listen_tcp(int server_port);
fd_set server_accept(fd_set master, int listener, int connfd, int *fdmax);
fd_set server_recv_send(fd_set master, int listener, int connfd, int fdmax);

#endif
