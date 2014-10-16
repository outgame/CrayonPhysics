#include "client.h"
//#include "utils.h"

#ifdef DEBUG
int main( int argc, char **argv ) {
    char *server = "127.0.0.1";
    int port = 5993;
    
    // all[0] = length of array
    int all[] = {7, 1, 3, 5, 7, 9, 11};
    char p[ all[0] * 4 ];
    
    int nbytes = client_encode(p, all);
    printf("sending %d bytes: %s\n", nbytes, p);
    
    int length, flag;
    char *text = client_decode_helper(p, &length, &flag);
    int rec[ length - 1 ]; // create the array with the length of the array sent
    
    int nints  = client_decode(rec, text);
    printf("received (%d ints)\n", nints);
    
    printf("Trying to connect to %s, port %d\n", server, port);
    int sockfd = client_socket_connect(server, port);
    
    if (sockfd < 0) {
        fprintf(stderr, "didn't connect (sockfd = %d)\n", sockfd);
        return EXIT_FAILURE;
    }
    
    printf("connected!\n");
    client_send(sockfd, "what's up guys?\n");
    
    //client_send(sockfd, hi);
    
    printf("Waiting to receive a string\n");
    char buf[MAXLINE];
    client_recv(sockfd, buf);
    printf("received: %s\n", buf);
    
    int length2, flag2;
    char *text2 = client_decode_helper(buf, &length2, &flag2);
    int rec2[ length2 - 1 ]; // create the array with the length of the array sent
    
    int nints2  = client_decode(rec2, text2);    
    printf("received (%d ints)\n", nints2);
    
    close(sockfd);
    return EXIT_SUCCESS;
}
#endif

int client_encode(char *buf, const int *all) { 
    char form[12]; // largest possible int, plus a char and \0
    
    for (int index = 0; index < all[0]; index++) {
        
        sprintf(form, "%d%c", all[index], SEPARATE);
        strcat(buf, form);
    }
    
    return strlen(buf);
}


char *client_decode_helper(char *pack, int *length, int *flag) {
    int first = 0;
    
    if (pack[0] < 0) {
        *flag = (int) pack[0];
        pack++;
    }
    
    while (pack[first] != SEPARATE) {
        first++;
    }
    
    char *endp = pack + first;
    *length = (int) strtol(pack, &endp, 10);
    
    return pack + first + 1; // skip the comma
}

int client_decode(int* all, char* pack) {
    
    int start = 0, end = 0;
    int counter = 0;
    char* endp;
    
    for (int index = 0; index < strlen(pack); index++) {
        
        if (pack[index] == SEPARATE) {
            end = index;
        }
        
        if (start < end) {
            endp = pack + end;
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

int client_socket_connect(const char *server, int port) {
    struct sockaddr_in servaddr;
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server);
    servaddr.sin_port =  htons(port);
    
    int sockfd;
    if (( sockfd = socket (AF_INET, SOCK_STREAM, 0) ) < 0) {
        fprintf(stderr, "client_socket_connect (socket)\n");
        return ERROR;
    }
    
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "client_socket_connect (connect)\n");
        return ERROR;
    }
    
    printf("client connecting to %s\n", server);
    return sockfd;
}

int client_recv(int sockfd, char *buf) {
    int nbytes = recv(sockfd, buf, MAXLINE - 1, 0);
    
    if (nbytes == -1) 
        #ifdef DEBUG
        fprintf(stderr, "client_recv\n");
        #endif
    
    buf[MAXLINE] = '\0'; // end the string with null
    return nbytes;
}

int client_send(int sockfd, const char *buf) {
    int len = strlen(buf);
    int nbytes = send( sockfd, buf, len, 0 );
    
#ifdef DEBUG
    if (nbytes == -1){
        fprintf(stderr, "client_send failed\n");
    }
#endif
    
    return nbytes;
}
