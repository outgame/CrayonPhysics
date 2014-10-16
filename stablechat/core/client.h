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

#define SEPARATE ','

int client_socket_connect(const char *server, int port);
int client_recv(int sockfd, char *buf);
int client_send(int sockfd, const char *buf);

int client_encode(char* buf, const int* all); // returns the number of bytes in the string
int client_decode(int* all, char* pack); // returns the length of the int array

char* client_decode_helper(char *pack, int *length, int *flag); // flag should be initialized to 0

#endif
