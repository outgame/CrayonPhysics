#include "client.h"

#ifdef DEBUG
int main( int argc, char **argv ) {
    char *server = "127.0.0.1";
    int port = 5993;
    
    printf("Trying to connect to %s, port %d\n", server, port);
    int sockfd = client_socket_connect(server, port);
    
    if (sockfd < 0) {
        fprintf(stderr, "didn't connect (sockfd = %d)\n", sockfd);
        return EXIT_FAILURE;
    }
    
    printf("connected!\n");
    client_send(sockfd, "what's up guys?\n");
    
    char buf[MAXLINE];
    char *p = client_recv(sockfd, buf);
    printf("received >%s\n", p);

    close(sockfd);
    return EXIT_SUCCESS;
}
#endif

int client_socket_connect(char *server, int port) {
    struct sockaddr_in servaddr;
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server);
    servaddr.sin_port =  htons(port);
    
    int sockfd;
    if (( sockfd = socket (AF_INET, SOCK_STREAM, 0) ) < 0) {
        fprintf(stderr, "client_socket_connect (socket)");
        return ERROR;
    }
    
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "client_socket_connect (connect)");
        return ERROR;
    }
    
    printf("client connecting to %s\n", server);
    return sockfd;
}

 int client_recv(int sockfd, char *buf) {
    int nbytes = recv(sockfd, buf, MAXLINE - 1, 0);
    
    if (nbytes == -1) {
        fprintf(stderr, "client_recv\n");
        return NULL;
    }
    
    buf[MAXLINE] = '\0'; // end the string with null
    return nbytes;
}

int client_send(int sockfd, char *buf) {
    int len = strlen(buf);
    int nbytes = send( sockfd, buf, len, 0 );
    
    if (nbytes == -1)
        fprintf(stderr, "client_send\n");
    
    return nbytes;
}
