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

/**
 * Bind a socket to create a connection with the server 
 * 
 * @param const char *server the address of the server
 * @param int port the port number that the server is running
 * @return int file descriptor of the bound socket
 */
int client_socket_connect(const char *server, int port);

/**
 * Receive data sent from the server
 *
 * @param int sockfd file descriptor of the socket connected to the server
 * @param char *buf empty buffer that will be filled with the received bytes
 * @return int number of bytes received from the server
 */
int client_recv(int sockfd, char *buf);

/**
 * Send a string to the server
 *
 * @param int sockfd file descriptor of the socket connected to the server
 * @param const char *buf to be sent to the server
 * @return int number of bytes sent
 */
int client_send(int sockfd, const char *buf);

/**
 * Encode an integer array to a string separated by a character
 * Example {4, 3, 11, 9} becomes "4,3,11,9,"
 *
 * @param char *buf empty buffer to be filled with the string
 * @param const int* all the array of integers to be encoded
 * @return int number of bytes in the string
 */
int client_encode(char* buf, const int* all);

/**
 * Prepares the string to be decoded by getting the length of the array
 * and reading in the encoded flag (the first char of the string) to know
 * if the string is a chat message or coordinates
 * If the first character has an ascii value of less than 0, the string contains coordinates
 * 
 * @param char* pack the encoded string
 * @param int* length to be set to the length of the array
 * @param int* flag to be set to the value of the encoded char at the beginning of the string
 */
char* client_decode_helper(char* pack, int* length, int* flag);

/**
 * Decodes a string filled with ints and stores them in an int array
 * 
 * @param int* all empty int array to be filled with ints from the string
 * @param char* pack the encoded string
 * @return int length of the int array
 */
int client_decode(int* all, char* pack);

#endif