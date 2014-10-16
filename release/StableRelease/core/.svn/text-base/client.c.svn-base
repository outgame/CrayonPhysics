#include "client.h"

#ifdef DEBUG

// test code for connecting to a server, and sending and receiving messages
int main( int argc, char **argv ) {
    // defaults to localhost, port 5993
    char *server = "127.0.0.1";
    int port = 5993;
    
    // all[0] = length of array
    int all[] = {7, 1, 3, 5, 7, 9, 11};
    char p[ all[0] * 4 ]; // room for 3 digit integers, plus a comma
    
    int nbytes = client_encode(p, all); // change the int array to a string
    printf("sending %d bytes: %s\n", nbytes, p);
    
    int length, flag;
    // length of string, flag is the first char in the string to know how to decode it
    char *text = client_decode_helper(p, &length, &flag);
    int rec[ length - 1 ]; // create the array with the length of the array sent
    
    int nints  = client_decode(rec, text);
    printf("received (%d ints)\n", nints);
    
    // connect to the server
    printf("Trying to connect to %s, port %d\n", server, port);
    int sockfd = client_socket_connect(server, port);
    
    if (sockfd < 0) { // no connection, sockfd == -1
        fprintf(stderr, "didn't connect (sockfd = %d)\n", sockfd);
        return EXIT_FAILURE;
    }
    
    // test sending and receiving
    printf("connected!\n"); 
    client_send(sockfd, "what's up guys?\n");
    
    printf("Waiting to receive a string\n");
    char buf[MAXLINE];
    client_recv(sockfd, buf);
    printf("received: %s\n", buf);
    
    // test decoding a string encoding a level sent from the server 
    int length2, flag2;
    char *text2 = client_decode_helper(buf, &length2, &flag2);
    int rec2[ length2 - 1 ]; // create the array with the length of the array sent
    
    int nints2  = client_decode(rec2, text2);    
    printf("received (%d ints)\n", nints2);
    
    // close the connection
    close(sockfd);
    return EXIT_SUCCESS;
}
#endif

int client_socket_connect(const char *server, int port) {
    struct sockaddr_in servaddr;
    
    // assumes INET
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server);
    servaddr.sin_port =  htons(port);
    
    // bind the socket and report an error if it fails
    int sockfd;
    if (( sockfd = socket (AF_INET, SOCK_STREAM, 0) ) < 0) {
        fprintf(stderr, "client_socket_connect (socket)\n");
        return ERROR;
    }
    
    // make the connection and report an error if it doesn't work
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "client_socket_connect (connect)\n");
        return ERROR;
    }
    
    printf("client connecting to %s\n", server);
    return sockfd; // file descriptor of the server
}

int client_recv(int sockfd, char *buf) {
    int nbytes = recv(sockfd, buf, MAXLINE - 1, 0);
    
    #ifdef DEBUG
    if (nbytes == -1) // if recv failed
        fprintf(stderr, "client_recv\n");
    #endif
    
    buf[MAXLINE] = '\0'; // end the string with null
    return nbytes;
}

int client_send(int sockfd, const char *buf) {
    int len = strlen(buf); // number of bytes to be sent
    int nbytes = send( sockfd, buf, len, 0 );
    
    #ifdef DEBUG
    if (nbytes == -1)
        fprintf(stderr, "client_send failed\n");
    #endif
    
    return nbytes;
}

int client_encode(char *buf, const int *all) { 
    char form[12]; // largest possible int, plus a char and \0
    
    for (int index = 0; index < all[0]; index++) {
        
        // add the current int and the SEPARATE character to the temporary buffer
        sprintf(form, "%d%c", all[index], SEPARATE);
        strcat(buf, form); // append to the main buffer
    }
    
    return strlen(buf); // return number of bytes in the string
}

char *client_decode_helper(char *pack, int *length, int *flag) {
    int first = 0; // index of the first comma in the string
    
    // if there is a character flag, set flag and increment the char pointer past it
    if (pack[0] < 0) {
        *flag = (int) pack[0];
        pack++;
    }
    
    // increment first until it hits a SEPARATE char
    while (pack[first] != SEPARATE) {
        first++;
    }
    
    char *endp = pack + first; // end pointer to SEPARATE
    *length = (int) strtol(pack, &endp, 10); // get the length of the array, base 10
    
    return pack + first + 1; // skip the SEPARATE
}

int client_decode(int* all, char* pack) {
    // start and end index of each int in the array
    int start = 0, end = 0;
    int counter = 0; // number of ints received
    char* endp;
    
    for (int index = 0; index < strlen(pack); index++) {
        
        if (pack[index] == SEPARATE) {
            // now start is the first digit and end is on SEPARATE
            end = index;
        }
        
        if (start < end) {
            // if end was just updated, move the end pointer to that char
            endp = pack + end;
            
            // get the int from pack and store it in all
            all[counter] = (int) strtol(pack + start, &endp, 10);
            
            #ifdef DEBUG
            printf("%d ", all[counter]);
            #endif
            
            start = end + 1; // skip the SEPARATE char
            counter++;
        }
    }
    
    return counter;
}