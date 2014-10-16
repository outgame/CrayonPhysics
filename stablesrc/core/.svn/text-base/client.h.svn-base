#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXLINE 256
#define ERROR -1

int client_socket_connect(char *server, int port);
int client_recv(int sockfd, char *buf);
int client_send(int sockfd, char *buf);

#endif
